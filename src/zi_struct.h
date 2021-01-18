#ifndef _ZI_STRUCT_H__
#define _ZI_STRUCT_H__

#include "build_config.h"
#include <cstdint>

/**
 * @brief 加密用的结构体。当需要自定义业务时，修改此结构体。同时修改FillHeader()函数
*/
struct ZiEncryptHdr
{
    uint32_t magic = 0x407A7963;
    uint32_t btime; // build time
    char ver_major, ver_minor, ver_patch;
};

// 若无inline，则会出现重复定义问题:
// multiple definition of `FillHeader(ZiEncryptHdr*)'
inline void FillHeader(struct ZiEncryptHdr* header)
{
    header->btime = BUILD_TIME;
    header->ver_major = DEMO_VER_MAJOR;
    header->ver_minor = DEMO_VER_MINOR;
    header->ver_patch = DEMO_VER_PATCH;
}

#endif /* _ZI_STRUCT_H__ */
