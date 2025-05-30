#pragma once
#include <cstdint>

namespace mdrivlib::SystemCache
{

inline void invalidate_dcache() {
}

inline void invalidate_icache() {
}

template<typename ptr>
inline void invalidate_dcache_by_addr(ptr addr) {
}

template<typename ptr>
inline void invalidate_dcache_by_addr_fast(ptr addr) {
}

inline void mem_barrier() {
}

inline void invalidate_dcache_by_range(void *addr, int32_t size) {
}

inline void clean_dcache() {
}

template<typename ptr>
inline void clean_dcache_by_addr(ptr addr) {
}

template<typename ptr>
inline void clean_dcache_by_addr_fast(ptr addr) {
}

inline void clean_dcache_by_range(void *addr, int32_t size) {
}

inline void clean_and_invalidate_dcache_by_range(void *addr, int32_t size) {
}
} // namespace mdrivlib::SystemCache
