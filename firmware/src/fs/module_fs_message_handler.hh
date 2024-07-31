#pragma once
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/inter_core_comm.hh"
#include "fs/fatfs/fat_file_io.hh"
#include <optional>

namespace MetaModule
{

struct ModuleFSMessageHandler {
	using MessageType = IntercoreModuleFSMessage::MessageType;
	using FatFsOp = IntercoreModuleFSMessage::FatFsOp;

	ModuleFSMessageHandler(IntercoreModuleFSMessage *shared_message_core0,
						   IntercoreModuleFSMessage *shared_message_core1)
		: intercore_comm_core0{*shared_message_core0}
		, intercore_comm_core1{*shared_message_core1} {
	}

	mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Responder, IntercoreModuleFSMessage, 2> intercore_comm_core0;
	mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Responder, IntercoreModuleFSMessage, 3> intercore_comm_core1;

	void process() {
		auto process_core = [this](auto &comm) {
			auto message = comm.get_new_message();
			if (message.message_type == MessageType::FatFsOpMessage) {
				pr_dbg("Got FatFsOpMessage\n");
				if (auto response = handle_fatfs_message(message)) {
					while (!comm.send_message(*response))
						;
				}
			} else if (message.message_type != MessageType::None) {
				pr_dbg("Got unknown msg %d\n", message.message_type);
			}
		};

		process_core(intercore_comm_core0);
		process_core(intercore_comm_core1);
	}

	std::optional<IntercoreModuleFSMessage> handle_fatfs_message(IntercoreModuleFSMessage &msg) {
		if (msg.fatfs_op == IntercoreModuleFSMessage::FatFsOp::Open) {
			pr_dbg("Got f_open");
			auto res = f_open(msg.fil, msg.path.data(), msg.mode);
			IntercoreModuleFSMessage response{
				.message_type = MessageType::FatFsOpMessage,
				.fatfs_req_id = msg.fatfs_req_id,
				.fatfs_op = msg.fatfs_op,
				.fil = msg.fil,
				.res = res,
			};
			pr_dbg("->%d\n", res);
			return response;

		} else {
			pr_err("Did not handle message with fatfs_op %d\n", msg.fatfs_op);
			return std::nullopt;
		}
	}
};

} // namespace MetaModule
