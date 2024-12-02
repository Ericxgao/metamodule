#include "filedesc_manager.hh"
#include <algorithm>
#include <array>
#include <optional>

namespace MetaModule::FileDescManager
{

static constexpr auto MaxOpenFiles = 64;

static std::array<FileDesc, MaxOpenFiles> descriptors{};

static bool fd_is_ok(int fd) {
	return (fd >= 0 && fd < MaxOpenFiles && descriptors[fd].fatfil != nullptr);
}

void init() {
	for (auto &d : descriptors) {
		d.fatfil = nullptr;
	}
}

std::optional<int> alloc_file() {

	auto it = std::find_if(descriptors.begin(), descriptors.end(), [](auto &f) { return f.fatfil == nullptr; });

	if (it != descriptors.end()) {
		auto fd = std::distance(descriptors.begin(), it);
		descriptors[fd].fatfil = new FIL;
		return fd;
	} else {
		return {};
	}
}

void dealloc_file(size_t fd) {
	if (descriptors[fd].fatfil) {
		delete descriptors[fd].fatfil;
		descriptors[fd].fatfil = nullptr;
	}
}

FileDesc *filedesc(size_t fd) {
	if (fd_is_ok(fd))
		return &descriptors[fd];
	else
		return nullptr;
}

}; // namespace MetaModule::FileDescManager
