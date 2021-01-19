#ifndef _DESERIALIZER_H__
#define _DESERIALIZER_H__

#include <cstdint>
#include "hs.h"

/**
 * @brief 从文件中反序列化得到DB作为返回值返回，若有额外头部信息可选择提取。
 * @param encrypted 是否有额外头部信息。
 * @param header_recv 若为nullptr，则表示对头信息不感兴趣，不保存头信息。
 * @return 得到的DB。返回 nullptr 表示反序列化失败。
*/
hs_database_t* ZiLoadDatabase(const char* dbfile, bool encrypted = false,
    struct ZiEncryptHdr* header_recv = nullptr);

/**
 * @param sc 单个或数组都通用。单个时记得指定 @p length 为1
*/
void ZiAllocScratchs(const hs_database_t* db, hs_scratch_t** sc, int length = 1,
    const char* info = "");

/**
 * @brief hs_scan封装
*/
uint32_t ZiScanDB(const char* content, const hs_database_t*, hs_scratch_t*);

/**
 * @brief hs_scan回调函数
*/
int ZiOnMatch(unsigned int id, unsigned long long from, unsigned long long to,
    unsigned int flags, void* ctx);


#endif /* _DESERIALIZER_H__ */
