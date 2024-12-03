#include "fs_syscall_proxy.hh"
#include "ff.h"

namespace MetaModule
{

FsSyscallProxy::FsSyscallProxy()
	: impl{std::make_unique<FsProxy>()} {
}

FsSyscallProxy::~FsSyscallProxy() = default;

bool FsSyscallProxy::open(std::string_view path, FIL *fil, uint8_t mode) {
	auto msg = IntercoreModuleFS::Open{
		.fil = *fil, //copy to non-cacheable Message
		.path = path,
		.access_mode = mode,
	};
	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Open>(msg, 3000)) {
		*fil = response->fil; //copy back
		return response->res == FR_OK;
	}
	return false;
}

uint64_t FsSyscallProxy::seek(FIL *fil, int offset, int whence) {
	auto msg = IntercoreModuleFS::Seek{
		.fil = *fil,
		.file_offset = (uint64_t)offset,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Seek>(msg, 3000)) {
		if (response->res == FR_OK) {
			*fil = response->fil;
		}
	}
	return -1;
}

std::optional<size_t> FsSyscallProxy::read(FIL *fil, std::span<char> buffer) {
	auto bytes_to_read = buffer.size();

	if (bytes_to_read > impl->file_buffer().size()) {
		pr_err("Cannot f_read %d bytes\n", bytes_to_read);
		return {};
	}

	auto msg = IntercoreModuleFS::Read{
		.fil = *fil,
		.buffer = impl->file_buffer().subspan(0, bytes_to_read),
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Read>(msg, 3000)) {
		std::copy(response->buffer.begin(), std::next(response->buffer.begin(), response->bytes_read), buffer.data());
		*fil = response->fil;
		return response->bytes_read;
	}

	return {};
}

int FsSyscallProxy::close(FIL *fil) {
	auto msg = IntercoreModuleFS::Close{
		.fil = *fil,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Close>(msg, 3000)) {
		*fil = response->fil; //copy FIL back
		return response->res;
	}

	return -1;
}

} // namespace MetaModule
