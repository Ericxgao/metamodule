#pragma once
#include "CoreModules/fs_access.hh"
#include "console/pr_dbg.hh"
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/cache.hh"
#include "drivers/inter_core_comm.hh"
#include "util/padded_aligned.hh"
#include <optional>

namespace MetaModule
{

namespace StaticBuffers
{
extern IntercoreModuleFSMessage icc_module_fs_message_core0;
extern IntercoreModuleFSMessage icc_module_fs_message_core1;
extern std::array<uint8_t, 65536> module_fs_buffer;
} // namespace StaticBuffers

struct FS::Impl {
	using enum IntercoreModuleFSMessage::MessageType;

public:
	Impl(std::string_view root)
		: root{root} {
	}

	static IntercoreModuleFSMessage make_close_message(FIL *fil) {
		return {
			.message_type = Close,
			.fil = fil,
		};
	}

	IntercoreModuleFSMessage make_open_message(FIL *fil, std::string_view filename, uint8_t read_mode) {
		mdrivlib::SystemCache::clean_dcache_by_range(&padded_file, sizeof(padded_file));

		return {
			.message_type = Open,
			.path = filename,
			.fil = &padded_file.data,
			.mode = read_mode,
		};
	}

	static IntercoreModuleFSMessage make_seek_message(FIL *fil, uint64_t offset) {
		return {
			.message_type = Seek,
			.fil = fil,
			.file_offset = offset,
		};
	}

	static IntercoreModuleFSMessage make_read_message(FIL *fil, std::span<char> buffer) {
		return {
			.message_type = Read,
			.buffer = buffer,
			.fil = fil,
		};
	}

	std::optional<IntercoreModuleFSMessage> get_response_or_timeout(IntercoreModuleFSMessage message,
																	uint32_t timeout = 3000) {
		auto request_type = message.message_type;

		auto start = HAL_GetTick();

		while (!send(message)) {

			if (HAL_GetTick() - start > 3000) {
				pr_dbg("Sending message timed out\n");
				return {}; //timeout
			}
		}

		start = HAL_GetTick();

		while (true) {
			if (auto msg = get_message(); msg.message_type != None) {
				if (msg.message_type == request_type) {

					mdrivlib::SystemCache::invalidate_dcache_by_range(&padded_file, sizeof(padded_file));
					return msg;

				} else {
					pr_dbg("Got unexpected response messsage type (%u vs %u)\n", msg.message_type, request_type);
					return {};
				}
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

	static bool send(IntercoreModuleFSMessage const &message) {
		if (core() == 1)
			return comm_core1.send_message(message);
		else
			return comm_core0.send_message(message);
	}

	static IntercoreModuleFSMessage get_message() {
		if (core() == 1)
			return comm_core1.get_new_message();
		else
			return comm_core0.get_new_message();
	}

	// Put these in separate place? They are used by FS, and hidden via pImpl
	std::string root;
	std::string cwd;

	PaddedAligned<FIL, 64> padded_file;
	PaddedAligned<DIR, 64> padded_dir;
	//buffer?

private:
	static constexpr uint32_t IPCC_ChanCore0 = 2;
	static constexpr uint32_t IPCC_ChanCore1 = 3;

	using CommModuleFS0 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFSMessage, IPCC_ChanCore0>;

	using CommModuleFS1 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFSMessage, IPCC_ChanCore1>;

	static inline CommModuleFS0 comm_core0{StaticBuffers::icc_module_fs_message_core0};
	static inline CommModuleFS1 comm_core1{StaticBuffers::icc_module_fs_message_core1};
};

} // namespace MetaModule
