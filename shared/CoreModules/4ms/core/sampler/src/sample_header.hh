#pragma once
#include "CoreModules/fs_access.hh"
#include "sample_type.hh"
#include <cstdint>

namespace SamplerKit
{
uint32_t load_sample_header(Sample *s_sample, FIL *sample_file);
}
