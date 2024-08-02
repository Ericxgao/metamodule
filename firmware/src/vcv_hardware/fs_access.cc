#include "CoreModules/fs_access.hh"
#include "console/pr_dbg.hh"
#include "core_intercom/intercore_modulefs_message.hh"
#include "drivers/inter_core_comm.hh"
#include "fs_access_impl.hh"
#include "util/padded_aligned.hh"
#include <cstdarg>
#include <cstring>
#include <optional>
#include <string_view>

namespace MetaModule
{

namespace StaticBuffers
{
extern IntercoreModuleFS::Message icc_module_fs_message_core0;
extern IntercoreModuleFS::Message icc_module_fs_message_core1;
} // namespace StaticBuffers

static constexpr bool print_fs_calls = false;
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

// READING

FRESULT FS::f_open(FIL *fp, const TCHAR *path, BYTE mode) {
	auto fullpath = impl->full_path(path);

	if (!write_access)
		mode &= ~(FA_WRITE | FA_CREATE_NEW | FA_CREATE_ALWAYS | FA_OPEN_APPEND);

	fs_trace("f_open(%p, \"%s\", %d)\n", fp, fullpath.c_str(), mode);

	auto msg = IntercoreModuleFS::Open{
		.fil = *fp,
		.path = std::string_view{fullpath},
		.access_mode = mode,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Open>(msg, 3000)) {
		*fp = response->fil; //copy FIL back
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_close(FIL *fil) {
	fs_trace("f_close(%p)\n", fil);

	auto msg = IntercoreModuleFS::Close{
		.fil = *fil,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Close>(msg, 3000)) {
		*fil = response->fil;
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_lseek(FIL *fil, uint64_t offset) {
	fs_trace("f_lseek(%p, %lld)\n", fil, offset);

	auto msg = IntercoreModuleFS::Seek{
		.fil = *fil,
		.file_offset = offset,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Seek>(msg, 3000)) {
		*fil = response->fil; //copy FIL back
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_read(FIL *fil, void *buff, UINT bytes_to_read, UINT *br) {
	fs_trace("f_read(%p, %p, %u, ...)\n", fil, buff, bytes_to_read);

	if (bytes_to_read > impl->file_buffer.size()) {
		pr_err("Cannot f_read %d bytes\n", bytes_to_read);
		return FR_NOT_ENOUGH_CORE;
	}

	auto msg = IntercoreModuleFS::Read{
		.fil = *fil,
		.buffer = impl->file_buffer.subspan(0, bytes_to_read),
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Read>(msg, 3000)) {
		*fil = response->fil;
		*br = response->bytes_read;
		std::copy(response->buffer.begin(), std::next(response->buffer.begin(), response->bytes_read), (char *)buff);

		return FR_OK;
	}
	return FR_TIMEOUT;
}

TCHAR *FS::f_gets(TCHAR *buffer, int len, FIL *fil) {
	fs_trace("f_gets(%p, %d, %p)\n", buffer, len, fil);

	if ((size_t)len > impl->file_buffer.size()) {
		pr_err("Cannot f_gets %d bytes\n", len);
		return nullptr;
	}

	auto msg = IntercoreModuleFS::GetS{
		.fil = *fil,
		.buffer = impl->file_buffer.subspan(0, len),
	};

	auto response = impl->get_response_or_timeout<IntercoreModuleFS::GetS>(msg, 3000);
	if (response) {
		*fil = response->fil;
		// copy buffer until we hit a \0
		std::strcpy(buffer, response->buffer.data());
		return msg.res;
	}
	return nullptr;
}

FRESULT FS::f_stat(const TCHAR *path, FILINFO *info) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_stat(%s, %p)\n", fullpath.c_str(), info);

	auto msg = IntercoreModuleFS::Stat{
		.path = fullpath.c_str(),
		// .info = *info, //will be overwritten TODO: check
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::Stat>(msg, 3000)) {
		*info = response->info;
		return response->res;
	}

	return FR_TIMEOUT;
}

// DIRS (READ-ONLY)

FRESULT FS::f_opendir(DIR *dir, const TCHAR *path) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_opendir(%p, %s)\n", dir, fullpath.c_str());

	auto msg = IntercoreModuleFS::OpenDir{
		.dir = *dir,
		.path = fullpath.c_str(),
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::OpenDir>(msg, 3000)) {
		*dir = response->dir;
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_closedir(DIR *dir) {
	fs_trace("f_closedir(%p)\n", dir);

	auto msg = IntercoreModuleFS::CloseDir{
		.dir = *dir,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::CloseDir>(msg, 3000)) {
		*dir = response->dir;
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_readdir(DIR *dir, FILINFO *info) {
	fs_trace("f_readdir(%p, %p)\n", dir, info);
	auto msg = IntercoreModuleFS::ReadDir{
		.dir = *dir,
		.info = *info,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::ReadDir>(msg, 3000)) {
		*dir = response->dir;
		*info = response->info;
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_findfirst(DIR *dir, FILINFO *info, const TCHAR *path, const TCHAR *pattern) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_findfirst(%p, %p, %s, %s)\n", dir, info, fullpath.c_str(), pattern);

	auto msg = IntercoreModuleFS::FindFirst{
		.dir = *dir,
		.info = *info,
		.path = fullpath.c_str(),
		.pattern = pattern,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::FindFirst>(msg, 3000)) {
		*dir = response->dir;
		*info = response->info;
		return response->res;
	}

	return FR_TIMEOUT;
}

FRESULT FS::f_findnext(DIR *dir, FILINFO *info) {
	fs_trace("f_findnext %p\n", dir);

	auto msg = IntercoreModuleFS::FindNext{
		.dir = *dir,
		.info = *info,
	};

	if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::FindNext>(msg, 3000)) {
		*dir = response->dir;
		*info = response->info;
		return response->res;
	}

	return FR_TIMEOUT;
}

// CREATE DIR

FRESULT FS::f_mkdir(const TCHAR *path) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_mkdir(%s)\n", fullpath.c_str());

	if (write_access) {
		auto msg = IntercoreModuleFS::MkDir{
			.path = fullpath.c_str(),
		};

		if (auto response = impl->get_response_or_timeout<IntercoreModuleFS::MkDir>(msg, 3000)) {
			return response->res;
		}
	}

	return FR_TIMEOUT;
}

// WRITING

FRESULT FS::f_write(FIL *fp, const void *buff, UINT btw, UINT *bw) {
	if (write_access) {
		fs_trace("f_write(%p, ...)\n", fp);
	}
	return FR_INT_ERR;
}

FRESULT FS::f_sync(FIL *fp) {
	fs_trace("f_sync(%p)\n", fp);
	if (write_access) {
	}
	return FR_INT_ERR;
}

FRESULT FS::f_truncate(FIL *fp) {
	fs_trace("f_truncate(%p)\n", fp);
	if (write_access) {
	}
	return FR_INT_ERR;
}

int FS::f_putc(TCHAR c, FIL *fp) {
	char s[2]{c, 0};
	return f_puts(s, fp);
}

int FS::f_puts(const TCHAR *str, FIL *fp) {
	if (write_access) {
		fs_trace("f_puts(\"%s\", %p)\n", str, fp);
	}
	return FR_INT_ERR;
}

int FS::f_printf(FIL *fp, const TCHAR *fmt, ...) {
	constexpr int MaxStringSize = 1024;

	va_list args;
	va_start(args, fmt);
	char buf[1 + MaxStringSize];
	auto sz = vsnprintf(buf, sizeof buf, fmt, args);
	if (sz > MaxStringSize)
		fs_trace("Tructuting f_printf to %zu chars\n", MaxStringSize);
	va_end(args);

	return f_puts(buf, fp);
}

FRESULT FS::f_expand(FIL *fp, FSIZE_t fsz, BYTE opt) {
	fs_trace("f_expand(%p...)\n", fp);
	if (write_access) {
	}
	return FR_INT_ERR;
}

// OTHER (write-access)

FRESULT FS::f_unlink(const TCHAR *path) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_unlink(%s)\n", fullpath.c_str());

	if (write_access) {
	}
	return FR_INT_ERR;
}

FRESULT FS::f_rename(const TCHAR *path_old, const TCHAR *path_new) {
	auto fullpath_old = impl->full_path(path_old);
	auto fullpath_new = impl->full_path(path_new);

	fs_trace("f_rename(%s, %s)\n", path_old, path_new);
	if (write_access) {
	}
	return FR_INT_ERR;
}

FRESULT FS::f_utime(const TCHAR *path, const FILINFO *fno) {
	auto fullpath = impl->full_path(path);

	fs_trace("f_utime(%s)\n", fullpath.c_str());

	if (write_access) {
	}
	return FR_INT_ERR;
}

// Working Dir:

FRESULT FS::f_chdir(const TCHAR *path) {
	fs_trace("f_chdir(%s)\n", path);

	//TODO: ensure ends in a slash
	impl->cwd = path;

	return FR_OK;
}

FRESULT FS::f_getcwd(TCHAR *buff, UINT len) {
	fs_trace("f_getcwd()\n");

	if (buff == nullptr)
		return FR_INVALID_PARAMETER;

	auto &cwd = impl->cwd;
	auto sz = std::min(len, cwd.size());
	std::copy(cwd.begin(), std::next(cwd.begin(), sz), buff);

	return FR_OK;
}

void FS::reset_dir(DIR *dp) {
	dp->obj.fs = nullptr;
}

void FS::reset_file(FIL *fp) {
	fp->obj.fs = nullptr;
	fp->cltbl = nullptr;
}

bool FS::is_file_reset(FIL *fp) {
	return fp->obj.fs == nullptr;
}

bool FS::f_eof(FIL *fp) {
	return fp->fptr == fp->obj.objsize;
}

BYTE FS::f_error(FIL *fp) {
	return fp->err;
}

FSIZE_t FS::f_tell(FIL *fp) {
	return fp->fptr;
}

FSIZE_t FS::f_size(FIL *fp) {
	return fp->obj.objsize;
}

FRESULT FS::f_rewind(FIL *fp) {
	return this->f_lseek(fp, 0);
}

FRESULT FS::f_rewinddir(DIR *dp) {
	return this->f_readdir(dp, nullptr);
}

FRESULT FS::f_rmdir(const TCHAR *path) {
	return this->f_unlink(path);
}

// Not supported:
//FRESULT FS::f_chdrive(const TCHAR *path);
// FRESULT FS::f_getfree(const TCHAR *path, DWORD *nclst, FATFS **fatfs); /* Get number of free clusters on the drive */
// FRESULT FS::f_getlabel(const TCHAR *path, TCHAR *label, DWORD *vsn);   /* Get volume label */
// FRESULT FS::f_setlabel(const TCHAR *label);							   /* Set volume label */
// FRESULT FS::f_mount(FATFS *fs, const TCHAR *path, BYTE opt); /* Mount/Unmount a logical drive */
// FRESULT FS::f_mkfs (const TCHAR* path, const MKFS_PARM* opt, void* work, UINT len);	/* Create a FAT volume */
// FRESULT FS::f_fdisk(BYTE pdrv, const LBA_t ptbl[], void *work); /* Divide a physical drive into some partitions */
// FRESULT FS::f_setcp(WORD cp);					  /* Set current code page */
// FRESULT FS::f_forward(FIL *fp, UINT (*func)(const BYTE *, UINT), UINT btf, UINT *bf); /* Forward data to the stream */

} // namespace MetaModule
