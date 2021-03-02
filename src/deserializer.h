#ifndef _ZI_DESERIALIZER_H__
#define _ZI_DESERIALIZER_H__

#include <cstdint>
#include "hs.h"

/**
 * @brief Get a db by deserializing a serialized file and return it.
 *      You can choose to extract the header info if the file do have a header.
 * @param has_header whether the @p dbfile carrys a header.
 * @param header_recver The header info to save when it isn't nullptr.
 * @return the generated database. nullptr if failed.
*/
hs_database_t* ZiLoadDatabase(const char* dbfile, bool has_header = false,
    struct ZiEncryptHdr* header_recver = nullptr);

/**
 * @param sc allocate @p length scratch(s) for @p db ,
 *      when more than one scratch is required, just pass a array, and set the
 *      @p length to the right value.
 * @param sc_length the number of scratch you need.
 * @param info information to print.
*/
void ZiAllocScratchs(const hs_database_t* db, hs_scratch_t** sc, int sc_len = 1,
    const char* info = "");

/**
 * @brief invoke @ref hs_scan .
 * @return 0 for Failed.
*/
uint32_t ZiScanDB(const char* content, const hs_database_t*, hs_scratch_t*);

/**
 * @brief call_back function of @ref ZiScanDB .
*/
int ZiOnMatch(unsigned int id, unsigned long long from, unsigned long long to,
    unsigned int flags, void* ctx);

#endif /* _ZI_DESERIALIZER_H__ */
