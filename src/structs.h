#ifndef _SERIALIZE_STRUCTS_H__
#define _SERIALIZE_STRUCTS_H__

#include <cstdint>

struct CzyDBInfo {
    uint32_t magic = 0x407A7963;
    uint32_t btime; // build time
    char ver_major, ver_minor, ver_sub;
};

#endif /* _SERIALIZE_STRUCTS_H__ */
