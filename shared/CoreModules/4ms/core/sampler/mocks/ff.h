#pragma once
#include <cstdint>

namespace MetaModule
{

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Access denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any problem */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > FF_FS_LOCK */
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} FRESULT;

struct FATFS;

typedef struct {
	FATFS *fs;	  /* Pointer to the hosting volume of this object */
	uint16_t id;  /* Hosting volume mount ID */
	uint8_t attr; /* Object attribute */
	uint8_t
		stat; /* Object chain status (b1-0: =0:not contiguous, =2:contiguous, =3:fragmented in this session, b2:sub-directory stretched) */
	uint32_t sclust;  /* Object data start cluster (0:no cluster or root directory) */
	uint64_t objsize; /* Object size (valid when sclust != 0) */
	uint32_t n_cont;  /* Size of first fragment - 1 (valid when stat == 3) */
	uint32_t n_frag;  /* Size of last fragment needs to be written to FAT (valid when not zero) */
	uint32_t c_scl;	  /* Containing directory start cluster (valid when sclust != 0) */
	uint32_t c_size;  /* b31-b8:Size of containing directory, b7-b0: Chain status (valid when c_scl != 0) */
	uint32_t c_ofs;	  /* Offset in the containing directory (valid when file object and sclust != 0) */
	uint32_t lockid;  /* File lock ID origin from 1 (index of file semaphore table Files[]) */
} FFOBJID;

typedef struct {
	FFOBJID obj;	   /* Object identifier (must be the 1st member to detect invalid object pointer) */
	uint8_t flag;	   /* File status flags */
	uint8_t err;	   /* Abort flag (error code) */
	uint64_t fptr;	   /* File read/write pointer (Zeroed on file open) */
	uint32_t clust;	   /* Current cluster of fpter (invalid when fptr is 0) */
	uint64_t sect;	   /* Sector number appearing in buf[] (0:invalid) */
	uint64_t dir_sect; /* Sector number containing the directory entry (not used at exFAT) */
	uint8_t *dir_ptr;  /* Pointer to the directory entry in the win[] (not used at exFAT) */
	uint32_t *cltbl;   /* Pointer to the cluster link map table (nulled on open, set by application) */
	uint8_t buf[512];  /* File private data read/write window */
} FIL;

typedef struct {
	FFOBJID obj;	  /* Object identifier */
	uint32_t dptr;	  /* Current read/write offset */
	uint32_t clust;	  /* Current cluster */
	uint64_t sect;	  /* Current sector (0:Read operation has terminated) */
	uint8_t *dir;	  /* Pointer to the directory item in the win[] */
	uint8_t fn[12];	  /* SFN (in/out) {body[8],ext[3],status[1]} */
	uint32_t blk_ofs; /* Offset of current entry block being processed (0xFFFFFFFF:Invalid) */
	const char *pat;  /* Pointer to the name matching pattern */
} DIR;

typedef struct {
	uint64_t fsize;		  /* File size */
	uint16_t fdate;		  /* Modified date */
	uint16_t ftime;		  /* Modified time */
	uint8_t fattrib;	  /* File attribute */
	char altname[12 + 1]; /* Altenative file name */
	char fname[512 + 1];  /* Primary file name */
} FILINFO;

/* File access mode and open method flags (3rd argument of f_open) */
#define FA_READ 0x01
#define FA_WRITE 0x02
#define FA_OPEN_EXISTING 0x00
#define FA_CREATE_NEW 0x04
#define FA_CREATE_ALWAYS 0x08
#define FA_OPEN_ALWAYS 0x10
#define FA_OPEN_APPEND 0x30

/* Fast seek controls (2nd argument of f_lseek) */
#define CREATE_LINKMAP ((uint64_t)0 - 1)

/* Filesystem type (FATFS.fs_type) */
#define FS_FAT12 1
#define FS_FAT16 2
#define FS_FAT32 3
#define FS_EXFAT 4

/* File attribute bits for directory entry (FILINFO.fattrib) */
#define AM_RDO 0x01 /* Read only */
#define AM_HID 0x02 /* Hidden */
#define AM_SYS 0x04 /* System */
#define AM_DIR 0x10 /* Directory */
#define AM_ARC 0x20 /* Archive */

} // namespace MetaModule
