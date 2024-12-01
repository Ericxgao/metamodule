#pragma once
#include "CoreModules/fs_access.hh"
#include <string>
#include <string_view>

namespace MetaModule
{

struct FSProxyImpl {
	std::string root;
	std::string cwd;

	Impl(std::string_view root)
		: root{root} {
	}

	std::string full_path(std::string_view path) {
		return root + std::string(path);
	}
};
} // namespace MetaModule
