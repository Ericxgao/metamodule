#pragma once
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/inter_core_comm.hh"
#include "fs/fatfs/fat_file_io.hh"
#include <optional>

namespace MetaModule
{

struct ModuleFSMessageHandler {
	using MessageType = IntercoreModuleFSMessage::MessageType;

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

			if (message.message_type != MessageType::None) {

				if (auto response = handle_fatfs_message(message)) {
					while (!comm.send_message(*response))
						;
				}
			}
		};

		process_core(intercore_comm_core0);
		process_core(intercore_comm_core1);
	}

	std::optional<IntercoreModuleFSMessage> handle_fatfs_message(IntercoreModuleFSMessage &msg) {
		switch (msg.message_type) {

			case MessageType::Open: {
				auto res = f_open(msg.fil, msg.path.data(), msg.mode);

				IntercoreModuleFSMessage response{
					.message_type = MessageType::Open,
					.fil = msg.fil,
					.res = res,
				};

				pr_dbg("M4: f_open(%p, %s, %d) -> %d\n", msg.fil, msg.path.data(), msg.mode, res);
				return response;
			} break;

			case MessageType::Seek: {
				auto res = f_lseek(msg.fil, msg.file_offset);

				IntercoreModuleFSMessage response{
					.message_type = MessageType::Seek,
					.fil = msg.fil,
					.res = res,
				};

				pr_dbg("M4: f_lseek(%p, %s, %d) -> %d\n", msg.fil, msg.path.data(), msg.mode, res);
				return response;

			} break;

			case MessageType::Read: {
				uint32_t bytes_read = 0;
				auto res = f_read(msg.fil, msg.buffer.data(), msg.buffer.size(), &bytes_read);

				IntercoreModuleFSMessage response{
					.message_type = MessageType::Seek,
					.fil = msg.fil,
					.res = res,
				};

				pr_dbg("M4: f_lseek(%p, %s, %d) -> %d\n", msg.fil, msg.path.data(), msg.mode, res);
				return response;

			} break;

			default:
				pr_dbg("M4: unknown Module FS msg %d\n", msg.message_type);
				break;
		}

		return std::nullopt;
	}
};

} // namespace MetaModule
