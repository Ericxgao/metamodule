#pragma once
#include "CoreModules/fs_access.hh"
#include "console/pr_dbg.hh"
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/cache.hh"
#include "drivers/inter_core_comm.hh"
#include "util/overloaded.hh"
#include "util/padded_aligned.hh"
#include <cstring>
#include <optional>

namespace MetaModule
{

namespace StaticBuffers
{
extern IntercoreModuleFS::Message icc_module_fs_message_core0;
extern IntercoreModuleFS::Message icc_module_fs_message_core1;
extern std::array<uint8_t, 64 * 1024> module_fs_buffer_core0;
extern std::array<uint8_t, 64 * 1024> module_fs_buffer_core1;
} // namespace StaticBuffers

struct FS::Impl {
public:
	// Put these in separate place? They are used by FS, and hidden via pImpl
	std::string root;
	std::string cwd;

	Impl(std::string_view root)
		: root{root} {

		auto &backing_buffer =
			core() == 1 ? StaticBuffers::module_fs_buffer_core1 : StaticBuffers::module_fs_buffer_core0;

		file_buffer = {(char *)backing_buffer.data(), backing_buffer.size()};
	}

public:
	FRESULT send_open_message(FIL *fp, std::string_view path, BYTE mode) {
		auto msg = IntercoreModuleFS::Open{
			.fil = *fp,
			.path = StaticString<255>{path},
			.access_mode = mode,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::Open>(msg, 3000)) {
			*fp = response->fil; //copy FIL back
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_close_message(FIL *fil) {
		auto msg = IntercoreModuleFS::Close{
			.fil = *fil,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::Close>(msg, 3000)) {
			*fil = response->fil; //copy FIL back
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_seek_message(FIL *fil, uint64_t offset) {
		auto msg = IntercoreModuleFS::Seek{
			.fil = *fil,
			.file_offset = offset,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::Seek>(msg, 3000)) {
			*fil = response->fil; //copy FIL back
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_read_message(FIL *fil, char *buff, unsigned bytes_to_read, unsigned *bytes_read) {
		if (bytes_to_read > file_buffer.size()) {
			pr_err("Cannot f_read %d bytes\n", bytes_to_read);
			return FR_NOT_ENOUGH_CORE;
		}

		auto msg = IntercoreModuleFS::Read{
			.fil = *fil,
			.buffer = file_buffer.subspan(0, bytes_to_read),
		};

		auto response = get_response_or_timeout<IntercoreModuleFS::Read>(msg, 3000);
		if (response) {
			*fil = response->fil;
			*bytes_read = response->bytes_read;
			std::copy(
				response->buffer.begin(), std::next(response->buffer.begin(), response->bytes_read), (char *)buff);
			return FR_OK;
		}
		return FR_TIMEOUT;
	}

	TCHAR *send_gets_message(FIL *fil, std::span<char> buffer) {
		if (buffer.size() > file_buffer.size()) {
			pr_err("Cannot f_gets %d bytes\n", buffer.size());
			return {};
		}

		auto msg = IntercoreModuleFS::GetS{
			.fil = *fil,
			.buffer = file_buffer.subspan(0, buffer.size()),
		};

		auto response = get_response_or_timeout<IntercoreModuleFS::GetS>(msg, 3000);
		if (response) {
			*fil = response->fil;
			// copy buffer until we hit a \0
			std::strcpy(buffer.data(), response->buffer.data());
			return msg.res;
		}
		return nullptr;
	}

	FRESULT send_stat_message(const TCHAR *path, FILINFO *info) {
		auto msg = IntercoreModuleFS::Stat{
			.path = path,
			// .info = *info, //will be overwritten TODO: check
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::Stat>(msg, 3000)) {
			*info = response->info;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_opendir_message(DIR *dir, const TCHAR *path) {
		auto msg = IntercoreModuleFS::OpenDir{
			.dir = *dir,
			.path = path,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::OpenDir>(msg, 3000)) {
			*dir = response->dir;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_closedir_message(DIR *dir) {
		auto msg = IntercoreModuleFS::CloseDir{
			.dir = *dir,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::CloseDir>(msg, 3000)) {
			*dir = response->dir;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_readdir_message(DIR *dir, FILINFO *info) {
		auto msg = IntercoreModuleFS::ReadDir{
			.dir = *dir,
			.info = *info,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::ReadDir>(msg, 3000)) {
			*dir = response->dir;
			*info = response->info;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_findfirst_message(DIR *dir, FILINFO *info, const TCHAR *path, const TCHAR *pattern) {
		auto msg = IntercoreModuleFS::FindFirst{
			.dir = *dir,
			.info = *info,
			.path = path,
			.pattern = pattern,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::FindFirst>(msg, 3000)) {
			*dir = response->dir;
			*info = response->info;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	FRESULT send_findnext_message(DIR *dir, FILINFO *info) {
		auto msg = IntercoreModuleFS::FindNext{
			.dir = *dir,
			.info = *info,
		};

		if (auto response = get_response_or_timeout<IntercoreModuleFS::FindNext>(msg, 3000)) {
			*dir = response->dir;
			*info = response->info;
			return response->res;
		}

		return FR_TIMEOUT;
	}

	template<typename ResponseT>
	std::optional<ResponseT> get_response_or_timeout(IntercoreModuleFS::Message const &message,
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

			// Ignore empty messages
			if (response.index() != IntercoreModuleFS::Message{IntercoreModuleFS::None{}}.index()) {

				if (auto type_response = std::get_if<ResponseT>(&response))
					return std::optional<ResponseT>(*type_response);
				else {
					pr_err("ModFS: Got unexpected response type (%u -> %u)\n", message.index(), response.index());
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

private:
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

	static constexpr uint32_t IPCC_ChanCore0 = 2;
	static constexpr uint32_t IPCC_ChanCore1 = 3;

	using CommModuleFS0 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore0>;

	using CommModuleFS1 =
		mdrivlib::InterCoreComm<mdrivlib::ICCRoleType::Initiator, IntercoreModuleFS::Message, IPCC_ChanCore1>;

	static inline CommModuleFS0 comm_core0{StaticBuffers::icc_module_fs_message_core0};
	static inline CommModuleFS1 comm_core1{StaticBuffers::icc_module_fs_message_core1};

	std::span<char> file_buffer;
};

} // namespace MetaModule
