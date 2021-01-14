#ifndef _DESERIALIZER_H__
#define _DESERIALIZER_H__

#include "structs.h"
#include "hs.h"

/**
 * @brief 从文件中反序列化得到DB作为返回值返回，并提取头部信息，保存到 @p header 中。
 * @param header 若为nullptr，则表示对头信息不感兴趣，不保存头信息。
 * @return 得到的DB。返回 nullptr 表示反序列化失败。
*/
hs_database_t* LoadDatabase(const char* dbfile, struct CzyDBInfo* header);

/**
 * @param sc 单个或数组都通用。单个时记得指定 @p length 为1
*/
void AllocHSScratch(const hs_database_t* db, hs_scratch_t** sc, int length = 1,
    const char* info = "");

// hs_scan封装
uint32_t ScanDB(const char* content, const hs_database_t*, hs_scratch_t*);

// hs_scan回调函数。
int OnMatch(unsigned int id, unsigned long long from, unsigned long long to,
    unsigned int flags, void* ctx);


#endif /* _DESERIALIZER_H__ */
