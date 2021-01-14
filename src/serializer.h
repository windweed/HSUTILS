/**
 * @brief 只用于生成db
*/

#ifndef _SERIALIZER_H__
#define _SERIALIZER_H__

#include <vector>
#include <string>
#include "hs.h"
#include <fstream>

struct HSCollData
{
    std::vector<std::string> patterns;
    std::vector<unsigned>    flags;
    std::vector<unsigned>    ids;
};

/**
 * @brief 将单个 @p db 保存成文件，文件名由 @p file 指定。
*/
bool ZiSaveDatabase(const hs_database_t* db, struct CzyDBInfo* header,
    const char* file);

hs_database_t* ZiBuildDatabase(const struct HSCollData& data,
    unsigned int mode, const char* info);

void FillHeader(struct CzyDBInfo* header);

void SplitString(const std::string& input, char delim,
    std::vector<std::string>& res);

void HsStr2Hex(const char* xstr, char* hexbuf);

void Getline_noCR(std::istream& is, std::string& s);

#endif /* _SERIALIZER_H__ */
