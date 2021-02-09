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
 * @brief hyperscan file to db. Take a look at @dir demo/
*/
struct ZiHSCollData
{
    std::vector<std::string> patterns;
    std::vector<unsigned>    flags;
    std::vector<unsigned>    ids;
};

/**
 * @brief serialize database to file named @p file .
 *      can be encrypted selectively.
 * @param header when not nullptr, write header's info, or, only serialize
*/
bool ZiSaveDatabase(const hs_database_t* db, struct ZiEncryptHdr* header,
    const char* file);

hs_database_t* ZiBuildDatabase(const struct ZiHSCollData& data,
    unsigned int mode, const char* info);

void SplitString(const std::string& input, char delim,
    std::vector<std::string>& res);

/**
 * @brief convert normal string to four characters: "\x12"...
*/
void HsStr2Hex(const char* xstr, char* hexbuf);

/**
 * @brief CR-safe std::getline<istream, string>()
*/
void Getline_noCR(std::istream& is, std::string& s);

#endif /* _SERIALIZER_H__ */
