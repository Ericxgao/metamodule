#pragma once
#include "ff.h"
#include "fs/volumes.hh"
#include "util/static_string.hh"
#include <span>
#include <string_view>

namespace MetaModule
{

struct IntercoreModuleFSMessage {
	enum MessageType : uint32_t {
		None,
		Open,
		Close,
		Read,
		Seek,
		Opendir,
		Closedir,
		Readdir,
		Stat,
	};

	MessageType message_type = MessageType::None;

	std::span<char> buffer;
	StaticString<255> path;

	FIL *fil{};
	DIR *dir{};
	uint8_t mode{};
	uint64_t file_offset{};
	uint32_t bytes_read{};
	FRESULT res;
};

} // namespace MetaModule
