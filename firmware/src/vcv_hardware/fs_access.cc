#include "CoreModules/fs_access.hh"

namespace MetaModule
{

FRESULT f_open(FIL *fp, const char *path, uint8_t mode) {
	return FR_INT_ERR;
}

FRESULT f_close(FIL *fp) {
	return FR_INT_ERR;
}

FRESULT f_read(FIL *fp, void *buff, unsigned btr, unsigned *br) {
	return FR_INT_ERR;
}

FRESULT f_write(FIL *fp, const void *buff, UINT btw, UINT *bw) {
	return FR_INT_ERR;
}

FRESULT f_lseek(FIL *fp, uint64_t ofs) {
	return FR_INT_ERR;
}

FRESULT f_truncate(FIL *fp) {
	return FR_INT_ERR;
}

FRESULT f_sync(FIL *fp) {
	return FR_INT_ERR;
}

FRESULT f_opendir(DIR *dp, const char *path) {
	return FR_INT_ERR;
}

FRESULT f_closedir(DIR *dp) {
	return FR_INT_ERR;
}

FRESULT f_readdir(DIR *dp, FILINFO *fno) {
	return FR_INT_ERR;
}

FRESULT f_findfirst(DIR *dp, FILINFO *fno, const TCHAR *path, const TCHAR *pattern) {
	return FR_INT_ERR;
}

FRESULT f_findnext(DIR *dp, FILINFO *fno) {
	return FR_INT_ERR;
}

FRESULT f_mkdir(const TCHAR *path) {
	return FR_INT_ERR;
}

FRESULT f_unlink(const TCHAR *path) {
	return FR_INT_ERR;
}

FRESULT f_rename(const TCHAR *path_old, const TCHAR *path_new) {
	return FR_INT_ERR;
}

FRESULT f_stat(const char *path, FILINFO *fno) {
	return FR_INT_ERR;
}

FRESULT f_chmod(const TCHAR *path, uint8_t attr, uint8_t mask) {
	return FR_INT_ERR;
}

FRESULT f_utime(const TCHAR *path, const FILINFO *fno) {
	return FR_INT_ERR;
}

FRESULT f_chdir(const TCHAR *path) {
	return FR_INT_ERR;
}

FRESULT f_chdrive(const TCHAR *path) {
	return FR_INT_ERR;
}

FRESULT f_getcwd(char *buff, uint32_t len) {
	return FR_INT_ERR;
}

// FRESULT f_getfree(const TCHAR *path, DWORD *nclst, FATFS **fatfs); /* Get number of free clusters on the drive */
// FRESULT f_getlabel(const TCHAR *path, TCHAR *label, DWORD *vsn);   /* Get volume label */
// FRESULT f_setlabel(const TCHAR *label);							   /* Set volume label */
// FRESULT f_forward(FIL *fp, UINT (*func)(const uint8_t *, UINT), UINT btf, UINT *bf); /* Forward data to the stream */
// FRESULT f_expand(FIL *fp, FSIZE_t fsz, uint8_t opt);		 /* Allocate a contiguous block to the file */
// FRESULT f_mount(FATFS *fs, const TCHAR *path, uint8_t opt); /* Mount/Unmount a logical drive */
// FRESULT f_mkfs (const TCHAR* path, const MKFS_PARM* opt, void* work, UINT len);	/* Create a FAT volume */
// FRESULT f_fdisk(uint8_t pdrv, const LBA_t ptbl[], void *work); /* Divide a physical drive into some partitions */
// FRESULT f_setcp(WORD cp);					  /* Set current code page */

int f_putc(TCHAR c, FIL *fp) {
	return FR_INT_ERR;
}

int f_puts(const TCHAR *str, FIL *cp) {
	return FR_INT_ERR;
}

int f_printf(FIL *fp, const TCHAR *str, ...) {
	return FR_INT_ERR;
}

char *f_gets(char *buff, int len, FIL *fp) {
	return buff;
}

#ifndef f_eof
#define f_eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
#endif

#ifndef f_error
#define f_error(fp) ((fp)->err)
#endif

#ifndef f_tell
#define f_tell(fp) ((fp)->fptr)
#endif

#ifndef f_size
#define f_size(fp) ((fp)->obj.objsize)
#endif

#ifndef f_rewind
#define f_rewind(fp) f_lseek((fp), 0)
#endif

#ifndef f_rewinddir
#define f_rewinddir(dp) f_readdir((dp), 0)
#endif

#ifndef f_rmdir
#define f_rmdir(path) f_unlink(path)
#endif

#ifndef f_unmount
#define f_unmount(path) f_mount(0, path, 0)
#endif

} // namespace MetaModule
