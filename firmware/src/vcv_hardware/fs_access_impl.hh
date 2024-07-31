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
		return IntercoreModuleFS::Close{
			.fil = *fil,
		};
	}

	IntercoreModuleFS::Message make_open_message(FIL *fil, std::string_view filename, uint8_t read_mode) {
		return IntercoreModuleFS::Open{
			.path = StaticString<255>{filename},
			.access_mode = read_mode,
		};
	}

	IntercoreModuleFS::Message make_seek_message(FIL *fil, uint64_t offset) {
		return IntercoreModuleFS::Seek{
			.fil = *fil,
			.file_offset = offset,
		};
	}

	IntercoreModuleFS::Message make_read_message(FIL *fil, unsigned bytes_to_read) {
		return IntercoreModuleFS::Read{
			.fil = *fil,
			.buffer = file_buffer.subspan(0, bytes_to_read),
		};
	}

	template<typename ResponseT>
	std::optional<ResponseT> get_response_or_timeout(IntercoreModuleFS::Message message, uint32_t timeout = 3000) {
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

			// Ignore None messages
			if (response.index() != IntercoreModuleFS::Message{IntercoreModuleFS::None{}}.index()) {

				if (auto type_response = std::get_if<ResponseT>(&response))
					return std::optional<ResponseT>(*type_response);
				else {
					pr_dbg("Got unexpected response messsage type (%u vs %u)\n", message.index(), response.index());
					return std::optional<ResponseT>{};
				}
			}

			if (HAL_GetTick() - start > timeout) {
				pr_dbg("Waiting for response timed out\n");
				return {};
			}
		}

		return {};
	}

	std::optional<IntercoreModuleFS::GetS> send_gets_message(FIL *fil, int len) {
		if (len <= 0 || len >= (int)file_buffer.size()) {
			pr_err("Cannot f_gets %d bytes\n", len);
			return {};
		}

		auto msg = IntercoreModuleFS::GetS{
			.fil = *fil,
			.buffer = file_buffer.subspan(0, len),
		};

		return get_response_or_timeout<IntercoreModuleFS::GetS>(msg, 3000);
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

private:
	static constexpr uint32_t IPCC_ChanCore0 = 2;
	static constexpr uint32_t IPCC_ChanCore1 = 3;

	using CommModuleFS0 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore0>;

	using CommModuleFS1 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore1>;

	static inline CommModuleFS0 comm_core0{StaticBuffers::icc_module_fs_message_core0};
	static inline CommModuleFS1 comm_core1{StaticBuffers::icc_module_fs_message_core1};
	static inline std::span<char> file_buffer{(char *)StaticBuffers::module_fs_buffer.data(),
											  StaticBuffers::module_fs_buffer.size()};
};

} // namespace MetaModule
