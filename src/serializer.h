/**
 * @brief for compiling database
*/

#ifndef _SERIALIZER_H__
#define _SERIALIZER_H__

#include <vector>
#include <string>
#include <fstream>
#include "hs.h"

/**
 * @brief hyperscan从文件编译database时需要的信息。
*/
struct ZiHSCollData
{
    std::vector<std::string> patterns;
    std::vector<unsigned>    flags;
    std::vector<unsigned>    ids;
};

/**
 * @brief 序列化接口， 将单个 @p db 保存成文件，文件名由 @p file 指定。 可选择加密。
 * @param header 非空时，加密，传入nullptr时表示不加密。
*/
bool ZiSaveDatabase(const hs_database_t* db, struct ZiEncryptHdr* header,
    const char* file);

hs_database_t* ZiBuildDatabase(const struct ZiHSCollData& data,
    unsigned int mode, const char* info);

void SplitString(const std::string& input, char delim,
    std::vector<std::string>& res);

void HsStr2Hex(const char* xstr, char* hexbuf);

void Getline_noCR(std::istream& is, std::string& s);

#endif /* _SERIALIZER_H__ */
