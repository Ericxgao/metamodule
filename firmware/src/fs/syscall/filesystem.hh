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
#include "filedesc_manager.hh"
#include "fs_proxy.hh"

namespace MetaModule
{

class Filesystem {
	static inline FatFileIO *mRamdisk{nullptr};
	static inline FSProxy fs_proxy;

public:
	static void init(FatFileIO &ramdisk) {
		mRamdisk = &ramdisk;
		FileDescManager::init();
	}

	static int open(const char *filename, int flags, int mode) {
		if (auto fd = FileDescManager::alloc_file()) {

			auto file = FileDescManager::filedesc(*fd);

			auto [path, volume] = split_volume(filename);

			if (volume == Volume::RamDisk) {
				if (mRamdisk) {
					if (mRamdisk->open(path, file->fatfil)) {
						return *fd;
					}
				}
			}

			if (volume == Volume::SDCard || volume == Volume::USB) {
				if (fs_proxy.open(path, file->fatfil, mode)) {
					return *fd;
				}
			}

			pr_err("Opening file failed\n");
			FileDescManager::dealloc_file(*fd);
			return -1;

		} else {
			pr_err("cannot open any more files\n");
			return -1;
		}
	}

	static int lseek(int fd, int offset, int whence) {
		if (auto file = FileDescManager::filedesc(fd)) {

			if (file->vol == Volume::RamDisk) {
				if (mRamdisk) {
					return mRamdisk->seek(file->fatfil, offset, whence);
				}
			}

			if (file->vol == Volume::SDCard || file->vol == Volume::USB) {
				return fs_proxy.seek(file->fatfil, offset, whence);
			}
		}
		return -1;
	}

	static int read(int fd, char *ptr, int len) {
		if (auto file = FileDescManager::filedesc(fd)) {

			auto buff = std::span{ptr, (size_t)len};

			if (file->vol == Volume::RamDisk) {
				if (mRamdisk) {
					const auto bytes_read = mRamdisk->read(file->fatfil, buff);
					return bytes_read.value_or(-1);
				}
			}

			if (file->vol == Volume::SDCard || file->vol == Volume::USB) {
				auto bytes_read = fs_proxy.read(file->fatfil, buff);
				return bytes_read.value_or(-1);
			}
		}

		return -1;
	}

	static int close(int fd) {
		if (auto file = FileDescManager::filedesc(fd)) {

			if (file->vol == Volume::RamDisk) {
				if (mRamdisk) {
					mRamdisk->close(file->fatfil);
				}
			}

			if (file->vol == Volume::USB || file->vol == Volume::SDCard) {
				fs_proxy.close(file->fatfil);
			}

			FileDescManager::dealloc_file(fd);

			return 0;
		}
		return -1;
	}

private:
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
};
} // namespace MetaModule
