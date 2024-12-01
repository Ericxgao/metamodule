#pragma once
#include "ff.h"
#include "fs/volumes.hh"
#include "util/static_string.hh"
#include <span>
#include <string_view>
#include <variant>

namespace MetaModule
{

namespace IntercoreModuleFS
{

struct None {};

// FIXME: why were are the FIL members FIL, not FIL*?
struct Open {
	FIL *fil{};
	StaticString<255> path;
	uint8_t access_mode{};
	FRESULT res{};
};

struct Close {
	FIL *fil{};
	FRESULT res{};
};

struct Read {
	FIL *fil{};
	std::span<char> buffer;
	uint32_t bytes_read{};
	FRESULT res{};
};

struct GetS {
	FIL fil{};
	std::span<char> buffer;
	uint32_t bytes_read{};
	char *res{};
};

struct Seek {
	FIL *fil{};
	uint64_t file_offset{};
	FRESULT res{};
};

struct OpenDir {
	DIR dir{};
	StaticString<255> path;
	FRESULT res{};
};

struct CloseDir {
	DIR dir{};
	FRESULT res{};
};

struct ReadDir {
	DIR dir{};
	FILINFO info{};
	FRESULT res{};
};

struct Stat {
	StaticString<255> path;
	FILINFO info{};
	FRESULT res{};
};

struct FindFirst {
	DIR dir{};
	FILINFO info{};
	StaticString<255> path;
	StaticString<63> pattern;
	FRESULT res{};
};

struct FindNext {
	DIR dir{};
	FILINFO info{};
	FRESULT res{};
};

struct MkDir {
	StaticString<255> path;
	FRESULT res{};
};

using Message =
	std::variant<None, Open, Close, Read, GetS, Seek, OpenDir, CloseDir, ReadDir, Stat, FindFirst, FindNext, MkDir>;

static constexpr size_t MessageSize = sizeof(Message);
//880B with DIR FIL FILINFO

} // namespace IntercoreModuleFS

} // namespace MetaModule
