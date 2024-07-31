#pragma once
#include "CoreModules/fs_access.hh"
#include "console/pr_dbg.hh"
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/cache.hh"
#include "drivers/inter_core_comm.hh"
#include "util/overloaded.hh"
#include "util/padded_aligned.hh"
#include <optional>

namespace MetaModule
{

namespace StaticBuffers
{
extern IntercoreModuleFS::Message icc_module_fs_message_core0;
extern IntercoreModuleFS::Message icc_module_fs_message_core1;
extern std::array<uint8_t, 65536> module_fs_buffer;
} // namespace StaticBuffers

struct FS::Impl {
	// using enum IntercoreModuleFS::Message::MessageType;

public:
	Impl(std::string_view root)
		: root{root} {
	}

	IntercoreModuleFS::Message make_close_message(FIL *fil) {
		active_fil = fil;
		return IntercoreModuleFS::Close{
			.fil = *fil,
		};
	}

	IntercoreModuleFS::Message make_open_message(FIL *fil, std::string_view filename, uint8_t read_mode) {
		//Store the fil ptr internally, and copy it back after we get a valid response
		active_fil = fil;
		return IntercoreModuleFS::Open{
			.path = StaticString<255>{filename},
			.access_mode = read_mode,
		};
	}

	IntercoreModuleFS::Message make_seek_message(FIL *fil, uint64_t offset) {
		active_fil = fil;
		return IntercoreModuleFS::Seek{
			.fil = *fil,
			.file_offset = offset,
		};
	}

	IntercoreModuleFS::Message make_read_message(FIL *fil, std::span<char> buffer, UINT *bytes_read) {
		active_fil = fil;
		active_buffer = buffer;
		active_bytes_read = bytes_read;
		return IntercoreModuleFS::Read{
			.fil = *fil,
			.buffer = file_buffer.subspan(0, buffer.size()),
		};
	}

	std::optional<IntercoreModuleFS::Message> get_response_or_timeout(IntercoreModuleFS::Message message,
																	  uint32_t timeout = 3000) {
		// Send
		auto start = HAL_GetTick();

		while (!send(message)) {

			if (HAL_GetTick() - start > 3000) {
				pr_dbg("Sending message timed out\n");
				return {}; //timeout
			}
		}

		// Get Response
		start = HAL_GetTick();

		while (true) {
			auto response = get_message();
			auto got_response = std::visit(overloaded{
											   [](IntercoreModuleFS::None &msg) { return false; },
											   [this](IntercoreModuleFS::Open &msg) {
												   *active_fil = msg.fil;
												   return true;
											   },
											   [this](IntercoreModuleFS::Seek &msg) {
												   *active_fil = msg.fil;
												   return true;
											   },
											   [this](IntercoreModuleFS::Read &msg) {
												   *active_fil = msg.fil;
												   std::copy(msg.buffer.begin(),
															 std::next(msg.buffer.begin(), msg.bytes_read),
															 active_buffer.begin());
												   *active_bytes_read = msg.bytes_read;
												   return true;
											   },
											   [this](IntercoreModuleFS::Close &msg) {
												   *active_fil = msg.fil;
												   return true;
											   },
											   [](auto &msg) {
												   pr_dbg("Got unknown response messsage\n");
												   return true;
											   },
										   },
										   response);

			if (got_response) {
				if (response.index() != message.index()) {
					pr_dbg("Got unexpected response messsage type (%u vs %u)\n", message.index(), response.index());
					return std::nullopt;
				}
				return response;
			}

			if (HAL_GetTick() - start > timeout) {
				pr_dbg("Waiting for response timed out\n");
				return {};
			}
		}

		return {};
	}

	static uint32_t core() {
		return __get_MPIDR() & 0b1;
	}

	static bool send(IntercoreModuleFS::Message const &message) {
		if (core() == 1)
			return comm_core1.send_message(message);
		else
			return comm_core0.send_message(message);
	}

	static IntercoreModuleFS::Message get_message() {
		if (core() == 1)
			return comm_core1.get_new_message();
		else
			return comm_core0.get_new_message();
	}

	// Put these in separate place? They are used by FS, and hidden via pImpl
	std::string root;
	std::string cwd;

	FIL *active_fil;
	std::span<char> active_buffer;
	UINT *active_bytes_read;

	static inline std::span<char> file_buffer{(char *)StaticBuffers::module_fs_buffer.data(),
											  StaticBuffers::module_fs_buffer.size()};
	// PaddedAligned<FIL, 64> padded_file;
	// PaddedAligned<DIR, 64> padded_dir;
	//buffer?

private:
	static constexpr uint32_t IPCC_ChanCore0 = 2;
	static constexpr uint32_t IPCC_ChanCore1 = 3;

	using CommModuleFS0 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore0>;

	using CommModuleFS1 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore1>;

	static inline CommModuleFS0 comm_core0{StaticBuffers::icc_module_fs_message_core0};
	static inline CommModuleFS1 comm_core1{StaticBuffers::icc_module_fs_message_core1};
};

} // namespace MetaModule
