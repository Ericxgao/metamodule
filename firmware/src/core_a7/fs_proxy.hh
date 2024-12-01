#include "ff.h"
#include "fs_proxy_impl.hh"

namespace MetaModule
{

// Goes between fs syscall wrappers and inter-core comunication.
// For SD and USB, not for RamDisk
// Better name?
class FSProxy {
public:
	FSProxy();
	~FSProxy();

	bool open(std::string_view path, FIL *fil, uint8_t mode);
	int close(FIL *fil);
	int seek(FIL *fil, int offset, int whence);
	std::optional<size_t> read(FIL *fil, std::span<char> buffer);

private:
	std::unique_ptr<FSProxyImpl> impl;
};

} // namespace MetaModule
