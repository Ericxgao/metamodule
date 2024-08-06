#pragma once
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace MetaModule
{

struct File {
	std::FILE *fil{};
	bool _reset = false;

	File() {
		reset();
	}

	void reset() {
		fclose(fil);
		_reset = true;
	}

	bool is_reset() {
		return _reset;
	}
};

struct Fileinfo {
	std::string filename;
	size_t filesize;
	enum class EntryType { Invalid, Dir, File } type{EntryType::Invalid};

	const char *fname() {
		return filename.c_str();
	}

	bool is_dir() {
		return type == EntryType::Dir;
	}
};

struct Dir {
	std::string path;
	std::vector<std::string> entries;
	size_t pos = 0;

	Dir() {
		reset();
	}

	void reset() {
		path.clear();
	}

	bool is_reset() {
		return (path.length() == 0);
	}
};

} // namespace MetaModule

using BYTE = uint8_t;
typedef unsigned int UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char BYTE; /* char must be 8-bit */
typedef uint16_t WORD;		/* 16-bit unsigned integer */
typedef uint32_t DWORD;		/* 32-bit unsigned integer */
typedef uint64_t QWORD;		/* 64-bit unsigned integer */
typedef WORD WCHAR;			/* UTF-16 character type */
typedef QWORD FSIZE_t;
typedef QWORD LBA_t;
typedef char TCHAR;

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

enum {
	FA_READ = 0x01,
	FA_WRITE = 0x02,
	FA_OPEN_EXISTING = 0x00,
	FA_CREATE_NEW = 0x04,
	FA_CREATE_ALWAYS = 0x08,
	FA_OPEN_ALWAYS = 0x10,
	FA_OPEN_APPEND = 0x30,
};

#define FF_MAX_LFN 255
// #define FF_LFN_BUF 255
