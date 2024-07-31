#pragma once
#include "CoreModules/fs_access.hh"
#include "console/pr_dbg.hh"
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/inter_core_comm.hh"
#include "util/padded_aligned.hh"
#include <optional>

namespace MetaModule
{

namespace StaticBuffers
{
extern IntercoreModuleFSMessage icc_module_fs_message_core0;
extern IntercoreModuleFSMessage icc_module_fs_message_core1;
} // namespace StaticBuffers

struct FS::Impl {
	using enum IntercoreModuleFSMessage::MessageType;
	using FatFsOp = IntercoreModuleFSMessage::FatFsOp;

public:
	Impl(std::string_view root)
		: root{root} {
	}

	uint32_t unique_id() {
		return last_uid++;
	}

	static uint32_t core() {
		return __get_MPIDR() & 0b1;
	}

	static std::optional<uint32_t> send(IntercoreModuleFSMessage const &message) {
		if (core() == 1)
			return comm_core1.send_message(message) ? message.fatfs_req_id : std::optional<uint32_t>{};
		else
			return comm_core0.send_message(message) ? message.fatfs_req_id : std::optional<uint32_t>{};
	}

	IntercoreModuleFSMessage make_close_message(FIL *fil) {
		return {
			.message_type = FatFsOpMessage,
			.fatfs_req_id = unique_id(),
			.fatfs_op = FatFsOp::Close,
			.fil = fil,
		};
	}

	IntercoreModuleFSMessage make_open_message(FIL *fil, std::string_view filename, uint8_t read_mode) {
		return {
			.message_type = FatFsOpMessage,
			.fatfs_req_id = unique_id(),
			.fatfs_op = FatFsOp::Open,
			.fil = fil,
			.mode = read_mode,
		};
	}

	IntercoreModuleFSMessage make_seek_message(FIL *fil, uint64_t offset) {
		return {
			.message_type = FatFsOpMessage,
			.fatfs_req_id = unique_id(),
			.fatfs_op = FatFsOp::Seek,
			.fil = fil,
			.file_offset = offset,
		};
	}

	IntercoreModuleFSMessage make_read_message(FIL *fil, std::span<char> buffer) {
		return {
			.message_type = FatFsOpMessage,
			.buffer = buffer,
			.fatfs_req_id = unique_id(),
			.fatfs_op = FatFsOp::Read,
			.fil = fil,
		};
	}

	// seek_message
	// read_message

	std::optional<IntercoreModuleFSMessage> get_response_or_timeout(IntercoreModuleFSMessage message,
																	uint32_t timeout = 3000) {
		auto start = HAL_GetTick();
		uint32_t msg_id{};

		while (true) {
			if (auto id = send(message); id) {
				msg_id = *id;
				break;
			}

			if (HAL_GetTick() - start > 3000) {
				pr_dbg("Sending message timed out\n");
				return {}; //timeout
			}
		}

		start = HAL_GetTick();

		while (true) {
			if (auto msg = get_message(); msg.message_type == FatFsOpMessage) {

				if (msg.fatfs_req_id == msg_id) {
					return msg;

				} else {
					pr_dbg("Got unexpected id (%u vs %u)\n", msg.fatfs_req_id, msg_id);
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

	IntercoreModuleFSMessage get_message() {
		if (core() == 1)
			return comm_core1.get_new_message();
		else
			return comm_core0.get_new_message();
	}

	std::string root;
	std::string cwd;

	PaddedAligned<FIL, 64> padded_file;

private:
	uint32_t last_uid = 999;

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
