#include "fs/time_convert.hh"

extern "C" uint32_t get_fattime() {
	return (uint32_t)ticks_to_fattime(0);
}
