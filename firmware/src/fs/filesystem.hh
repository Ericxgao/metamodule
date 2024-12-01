#pragma once

// fopen()
// -> newlib/libc++
// -> _open()
// -> MetaModule::Filesystem::open
// Ramdisk -> RamDisk::open()
//         -> FatFileIO::open()
// 		   -> f_open() .... fatfs ops
//
// USB/SD  -> FSProxy::open
//         -> FSProxyImpl::
//         -> Core M4 handler

#include "fat_file_io.hh"
#include "ff.h"
#include "fs_proxy.hh"
#include <algorithm>

namespace MetaModule
{

class Filesystem {
	static constexpr auto max_open_files = 64;

	struct File {
		FIL *fatfil;
		//TODO: LFS::TimeFile *lfsfil;
		Volume vol;
	};

	static inline std::array<File, max_open_files> descriptors{};
	static inline FatFileIO *mRamdisk{nullptr};
	static inline FSProxy fs_proxy;

public:
	static void init(FatFileIO &ramdisk) {
		mRamdisk = &ramdisk;
		for (auto &d : descriptors) {
			d.fatfil = nullptr;
		}
	}

	static int open(const char *filename, int flags, int mode) {
		if (auto fd = first_available_fd_idx()) {
			auto [path, volume] = split_volume(filename);

			descriptors[*fd].fatfil = new FIL;

			if (volume == Volume::RamDisk) {
				if (mRamdisk) {
					if (mRamdisk->open(path, descriptors[*fd].fatfil)) {
						return *fd;
					}
				}
			}
			if (volume == Volume::SDCard || volume == Volume::USB) {
				if (fs_proxy.open(path, descriptors[*fd].fatfil, mode)) {
					return *fd;
				}
			}

			pr_err("Opening file failed\n");
			delete descriptors[*fd].fatfil;
			descriptors[*fd].fatfil = nullptr;
			return -1;

		} else {
			pr_err("cannot open any more files, max == %d", max_open_files);
			return -1;
		}
	}

	static int lseek(int fd, int offset, int whence) {
		if (fd_is_ok(fd)) {

			auto volume = descriptors[fd].vol;

			if (volume == Volume::RamDisk) {
				if (mRamdisk) {
					return mRamdisk->seek(descriptors[fd].fatfil, offset, whence);
				}
			}

			if (volume == Volume::SDCard || volume == Volume::USB) {
				return fs_proxy.seek(descriptors[fd].fatfil, offset, whence);
			}
		}
		return -1;
	}

	static int read(int fd, char *ptr, int len) {
		if (fd_is_ok(fd)) {

			auto buff = std::span{ptr, (size_t)len};

			auto volume = descriptors[fd].vol;

			if (volume == Volume::RamDisk) {
				if (mRamdisk) {
					const auto bytes_read = mRamdisk->read(descriptors[fd].fatfil, buff);
					return bytes_read.value_or(-1);
				}
			}

			if (volume == Volume::SDCard || volume == Volume::USB) {
				auto bytes_read = fs_proxy.read(descriptors[fd].fatfil, buff);
				return bytes_read.value_or(-1);
			}
		}

		return -1;
	}

	static int close(int fd) {
		if (fd_is_ok(fd)) {

			auto volume = descriptors[fd].vol;

			if (volume == Volume::RamDisk) {
				if (mRamdisk) {
					mRamdisk->close(descriptors[fd].fatfil);
				}
			}
			if (volume == Volume::USB || volume == Volume::SDCard) {
				fs_proxy.close(descriptors[fd].fatfil);
			}

			if (descriptors[fd].fatfil) {
				delete descriptors[fd].fatfil;
				descriptors[fd].fatfil = nullptr;
			}

			return 0;
		}
		return -1;
	}

private:
	static bool fd_is_ok(int fd) {
		return (fd >= 0 && fd < max_open_files && descriptors[fd].fatfil != nullptr);
	}

	static std::pair<std::string_view, Volume> split_volume(const char *filename) {
		auto sv = std::string_view{filename};

		if (sv.starts_with("/ram/"))
			return {sv.substr(5), Volume::RamDisk};

		if (sv.starts_with("/usb/"))
			return {sv.substr(5), Volume::USB};

		if (sv.starts_with("/sdc/"))
			return {sv.substr(5), Volume::SDCard};

		if (sv.starts_with("/nor/"))
			return {sv.substr(5), Volume::NorFlash};

		// Default (no volume given) is RamDisk
		return {sv, Volume::RamDisk};
	}

	static std::optional<int> first_available_fd_idx() {
		auto it = std::find_if(descriptors.begin(), descriptors.end(), [](auto &f) { return f.fatfil == nullptr; });
		if (it != descriptors.end()) {
			return std::distance(descriptors.begin(), it);
		} else {
			return {};
		}
	}
};
} // namespace MetaModule
