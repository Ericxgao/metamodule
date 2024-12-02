#pragma once
#include "ff.h"
#include "volumes.hh"
#include <optional>

namespace MetaModule
{

struct FileDesc {
	FIL *fatfil = nullptr;
	Volume vol{};
};

namespace FileDescManager
{

void init();
std::optional<int> alloc_file();
void dealloc_file(size_t fd);
FileDesc *filedesc(size_t fd);

} // namespace FileDescManager
} // namespace MetaModule
