#ifndef _ENCRYPT_HDR_H__
#define _ENCRYPT_HDR_H__

#include "build_config.h"
#include <cstdint>

/**
 * @brief to encrypt.
*/
struct ZiEncryptHdr
{
    uint32_t magic = 0x407A7963;
    uint32_t btime; // build time
    char ver_major, ver_minor, ver_patch;

    ZiEncryptHdr()
        : btime {BUILD_TIME}
        , ver_major {VER_MAJOR}
        , ver_minor {VER_MINOR}
        , ver_patch {VER_PATCH}
    {}
};

#endif /* _ENCRYPT_HDR_H__ */
