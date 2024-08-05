#include "CoreModules/fs_access.hh"
#include "fs/fs_access_impl.hh"
#include <array>
#include <cstdarg>
#include <cstring>
#include <rack.hpp>
#include <string_view>

extern rack::plugin::Plugin *pluginInstance;

namespace MetaModule
{

static constexpr bool print_fs_calls = true;
static constexpr bool write_access = false;

static inline void fs_trace(const char *str) {
	if constexpr (print_fs_calls)
		printf("%s", str);
}

static inline void fs_trace(auto... args) {
	if constexpr (print_fs_calls)
		printf(args...);
}

FS::FS(std::string_view root)
	: impl{new Impl(root)} {
}

FS::~FS() = default;

// Valid Root

bool FS::find_valid_root(std::string_view path) {
	std::string userdir = rack::asset::user("");
	std::string userdir2 = rack::asset::plugin(pluginInstance, "");

	std::array<std::string, 2> valid_roots{
		userdir,
		userdir2,
	};

	auto t_root = impl->root;
	auto t_cwd = impl->cwd;

	for (auto root : valid_roots) {
		impl->root = root;
		impl->cwd = "./";

		File file;
		auto res = f_open(&file, path.data(), FA_READ);
		f_close(&file);
		if (res == FR_OK) {
			printf("Found valid root %s\n", root.data());
			//keep the root, restore cwd
			impl->cwd = t_cwd;
			return true;
		}
	}

	// no valid root found. restore previous values
	impl->root = t_root;
	impl->cwd = t_cwd;
	printf("No valid root found\n");

	return false;
}

// READING

FRESULT FS::f_open(File *fp, const char *path, uint8_t mode) {
	auto fullpath = impl->full_path(path);

	if (!write_access)
		mode &= ~(FA_WRITE | FA_CREATE_NEW | FA_CREATE_ALWAYS | FA_OPEN_APPEND);

	fs_trace("f_open(%p, \"%s\", %d)\n", fp, fullpath.c_str(), mode);

	std::string posixmode = mode == (FA_READ)								? "r" :
							mode == (FA_READ | FA_WRITE)					? "r+" :
							mode == (FA_CREATE_ALWAYS | FA_WRITE)			? "w" :
							mode == (FA_CREATE_ALWAYS | FA_WRITE | FA_READ) ? "w+" :
							mode == (FA_OPEN_APPEND | FA_WRITE)				? "a" :
							mode == (FA_OPEN_APPEND | FA_WRITE | FA_READ)	? "a+" :
							mode == (FA_CREATE_NEW | FA_WRITE)				? "wx" :
							mode == (FA_CREATE_NEW | FA_WRITE | FA_READ)	? "w+x" :
																			  "";

	fp->fil = std::fopen(fullpath.c_str(), "rb"); //posixmode.c_str());

	if (fp->fil == nullptr)
		return FR_NO_FILE;
	else
		return FR_OK;
}

FRESULT FS::f_close(File *fil) {
	fs_trace("f_close(%p)\n", fil);

	auto res = std::fclose(fil->fil);
	if (res == 0)
		return FR_OK;
	else
		return FR_DISK_ERR;
}

FRESULT FS::f_lseek(File *fil, uint64_t offset) {
	if (offset == 18446744073709551595ULL) {
		fs_trace("f_lseek request for cluster map, ignored\n");
		return FR_OK;
	}

	fs_trace("f_lseek(%p, %llu)\n", fil, offset);

	auto res = std::fseek(fil->fil, offset, SEEK_SET);

	return res == 0 ? FR_OK : FR_TIMEOUT;
}

FRESULT FS::f_read(File *fil, void *buff, unsigned bytes_to_read, unsigned *br) {
	fs_trace("f_read(%p, %p, %u, ...)\n", fil, buff, bytes_to_read);

	auto bytes_read = std::fread(buff, 1, bytes_to_read, fil->fil);
	*br = bytes_read;

	if (bytes_read == 0 && bytes_to_read != 0)
		return FR_NO_FILE;
	else
		return FR_OK;
}

char *FS::f_gets(char *buffer, int len, File *fil) {
	fs_trace("f_gets(%p, %d, %p)\n", buffer, len, fil);

	return std::fgets(buffer, len, fil->fil);
}

FRESULT FS::f_stat(const char *path, Fileinfo *info) {
	if (info == nullptr)
		return FR_INVALID_PARAMETER;

	auto fullpath = impl->full_path(path);

	fs_trace("[NOT IMPL] f_stat(%s, %p)\n", fullpath.c_str(), info);

	// stat p_stat;

	// info->fsize = buf->st_size = info.fsize;
	// epoch = fat_time_to_unix(info.fdate, info.ftime);
	// buf->st_atime = epoch;                        // Access time
	// buf->st_mtime = epoch;                        // Modification time
	// buf->st_ctime = epoch;                        // Creation time

	// // We only handle read only case
	// mode = (FATFS_R | FATFS_X);
	// if( !(info.fattrib & AM_RDO))
	// mode |= (FATFS_W);                        // enable write if NOT read only

	// if(info.fattrib & AM_SYS)
	// {
	// buf->st_uid= 0;
	// buf->st_gid= 0;
	// }
	// {
	// buf->st_uid=1000;
	// buf->st_gid=1000;
	// }

	// if(info.fattrib & AM_DIR)
	// mode |= S_IFDIR;
	// else
	// mode |= S_IFREG;
	// buf->st_mode = mode;

	return FR_TIMEOUT;
}

// DIRS (READ-ONLY)

FRESULT FS::f_opendir(Dir *dir, const char *path) {
	auto fullpath = impl->full_path(path);

	fs_trace("[NOT IMPL] f_opendir(%p, %s)\n", dir, fullpath.c_str());
	// dir->dir = opendir(path);
	// return dir->dir == nullptr ? FR_NO_FILE : FR_OK;
	return FR_INT_ERR;
}

FRESULT FS::f_closedir(Dir *dir) {
	if (!dir)
		return FR_INVALID_PARAMETER;

	fs_trace("[NOT IMPL] f_closedir(%p)\n", dir);
	// return closedir(dir->dir) == 0 ? FR_OK : FR_DISK_ERR;
	return FR_INT_ERR;
}

FRESULT FS::f_readdir(Dir *dir, Fileinfo *info) {
	fs_trace("[NOT IMPL] f_readdir(%p, %p)\n", dir, info);
	return FR_TIMEOUT;
}

FRESULT FS::f_findfirst(Dir *dir, Fileinfo *info, const char *path, const char *pattern) {
	auto fullpath = impl->full_path(path);
	fs_trace("[NOT IMPL] f_findfirst(%p, %p, %s, %s)\n", dir, info, fullpath.c_str(), pattern);

	return FR_TIMEOUT;
}

FRESULT FS::f_findnext(Dir *dir, Fileinfo *info) {
	fs_trace("[NOT IMPL] f_findnext %p\n", dir);
	return FR_TIMEOUT;
}

// CREATE Dir

FRESULT FS::f_mkdir(const char *path) {
	auto fullpath = impl->full_path(path);

	if (write_access) {
		// fs_trace("f_mkdir(%s)\n", fullpath.c_str());
		// return mkdir(path, 0700) == 0 ? FR_OK : FR_INT_ERR;
	}

	return FR_TIMEOUT;
}

// WRITING

FRESULT FS::f_write(File *fp, const void *buff, unsigned btw, unsigned *bw) {
	if (write_access) {
		fs_trace("[NOT IMPL] f_write(%p, ...)\n", fp);
	}
	return FR_INT_ERR;
}

FRESULT FS::f_sync(File *fp) {
	fs_trace("f_sync(%p)\n", fp);
	if (write_access) {
		return fflush(fp->fil) == 0 ? FR_OK : FR_INT_ERR;
	}
	return FR_INT_ERR;
}

FRESULT FS::f_truncate(File *fp) {
	fs_trace("f[NOT IMPL] _truncate(%p)\n", fp);
	if (write_access) {
	}
	return FR_INT_ERR;
}

int FS::f_putc(char c, File *fp) {
	char s[2]{c, 0};
	return f_puts(s, fp);
}

int FS::f_puts(const char *str, File *fp) {
	if (write_access) {
		fs_trace("[NOT IMPL] f_puts(\"%s\", %p)\n", str, fp);
	}
	return FR_INT_ERR;
}

int FS::f_printf(File *fp, const char *fmt, ...) {
	constexpr int MaxStringSize = 1024;

	va_list args;
	va_start(args, fmt);
	char buf[1 + MaxStringSize];
	auto sz = vsnprintf(buf, sizeof buf, fmt, args);
	if (sz > MaxStringSize)
		fs_trace("[NOT IMPL] Tructuting f_printf to %zu chars\n", MaxStringSize);
	va_end(args);

	return f_puts(buf, fp);
}

FRESULT FS::f_expand(File *fp, FSIZE_t fsz, uint8_t opt) {
	fs_trace("[NOT IMPL] f_expand(%p...)\n", fp);
	if (write_access) {
	}
	return FR_INT_ERR;
}

// OTHER (write-access)

FRESULT FS::f_unlink(const char *path) {
	auto fullpath = impl->full_path(path);

	fs_trace("[NOT IMPL] f_unlink(%s)\n", fullpath.c_str());

	if (write_access) {
	}
	return FR_INT_ERR;
}

FRESULT FS::f_rename(const char *path_old, const char *path_new) {
	auto fullpath_old = impl->full_path(path_old);
	auto fullpath_new = impl->full_path(path_new);

	fs_trace("[NOT IMPL] f_rename(%s, %s)\n", path_old, path_new);
	if (write_access) {
	}
	return FR_INT_ERR;
}

FRESULT FS::f_utime(const char *path, const Fileinfo *fno) {
	auto fullpath = impl->full_path(path);

	fs_trace("[NOT IMPL] f_utime(%s)\n", fullpath.c_str());

	if (write_access) {
	}
	return FR_INT_ERR;
}

// Working Dir:

FRESULT FS::f_chdir(const char *path) {
	fs_trace("f_chdir(%s)\n", path);

	//TODO: ensure ends in a slash
	impl->cwd = path;

	return FR_OK;
}

FRESULT FS::f_getcwd(char *buff, unsigned len) {
	fs_trace("f_getcwd()\n");

	if (buff == nullptr)
		return FR_INVALID_PARAMETER;

	auto &cwd = impl->cwd;
	auto sz = std::min<size_t>(len, cwd.size());
	std::copy(cwd.begin(), std::next(cwd.begin(), sz), buff);

	return FR_OK;
}

void FS::reset_dir(Dir *dp) {
	// return (dp == nullptr || dp->dir == nullptr);
}

void FS::reset_file(File *fp) {
	// fp->obj.fs = nullptr;
	// fp->cltbl = nullptr;
}

bool FS::is_file_reset(File *fp) {
	return (fp == nullptr || fp->fil == nullptr);
}

bool FS::f_eof(File *fp) {
	if (fp)
		return feof(fp->fil) != 0;
	else
		return false;
}

uint8_t FS::f_error(File *fp) {
	if (fp)
		return ferror(fp->fil);
	else
		return FR_INVALID_PARAMETER;
}

FSIZE_t FS::f_tell(File *fp) {
	return ftell(fp->fil);
}

FSIZE_t FS::f_size(File *fp) {
	if (!fp)
		return 0;

	// Get original position
	auto pos = std::ftell(fp->fil);

	std::fseek(fp->fil, 0, SEEK_END);
	auto filesize = std::ftell(fp->fil);

	// Restore original position
	std::fseek(fp->fil, pos, SEEK_SET);

	return filesize;
}

FRESULT FS::f_rewind(File *fp) {
	if (!fp)
		return FR_NO_FILE;

	return fseek(fp->fil, 0, SEEK_SET) == 0 ? FR_OK : FR_INT_ERR;
}

FRESULT FS::f_rewinddir(Dir *dp) {
	//[NOT_IMPL]
	return FR_INT_ERR;
}

FRESULT FS::f_rmdir(const char *path) {
	//[NOT_IMPL]
	return FR_INT_ERR;
}

//FRESULT FS::f_chdrive(const char *path);
// FRESULT FS::f_getfree(const char *path, DWORD *nclst, FATFS **fatfs); /* Get number of free clusters on the drive */
// FRESULT FS::f_getlabel(const char *path, char *label, DWORD *vsn);   /* Get volume label */
// FRESULT FS::f_setlabel(const char *label);							   /* Set volume label */
// FRESULT FS::f_mount(FATFS *fs, const char *path, uint8_t opt); /* Mount/Unmount a logical drive */
// FRESULT FS::f_mkfs (const char* path, const MKFS_PARM* opt, void* work, unsigned len);	/* Create a FAT volume */
// FRESULT FS::f_fdisk(uint8_t pdrv, const LBA_t ptbl[], void *work); /* Divide a physical drive into some partitions */
// FRESULT FS::f_setcp(WORD cp);					  /* Set current code page */
// FRESULT FS::f_forward(FIL *fp, unsigned (*func)(const uint8_t *, unsigned), unsigned btf, unsigned *bf); /* Forward data to the stream */

} // namespace MetaModule
