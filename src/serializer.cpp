#include "build_config.h"
#include "serializer.h"
#include "structs.h"
#include <fstream>
#include <vector>
#include <cstring> // strlen()

using namespace std;

// 带有头部信息的序列化接口
bool SaveDatabase(const hs_database_t* db, struct CzyDBInfo* header,
    const char* file)
{
    printf("[ Info ] Serializing database to file: '%s'\n", file);

    char* serialized_db = nullptr;
    size_t sdb_length = 0;
    hs_error_t err = hs_serialize_database(db, &serialized_db, &sdb_length);
    if (err != HS_SUCCESS)
    {
        fprintf(stderr, "[ ERROR ] Serialize Failed with: '%d'\n", err);
        return false;
    }

    printf("[ Info ] Serialized Size: '%zu'\n", sdb_length);

    ofstream out(file, ios::binary);
    out.write((char*) header, sizeof(*header));
    out.write(serialized_db, sdb_length);
    out.close();

    free(serialized_db);
    return true;
}


void FillHeader(struct CzyDBInfo* header)
{
    header->btime = BUILD_TIME;
    header->ver_major = VER_MAJOR;
    header->ver_minor = VER_MINOR;
    header->ver_sub = VER_SUB;
}

hs_database_t* BuildDatabase(const struct HSCollData& data, unsigned int mode,
    const char* info)
{
    if (data.patterns.empty())
    {
        fprintf(stderr, "[ Info ] No '%s' Hyperscan Rules!\n", info);
        return nullptr;
    }

    vector<const char*> exprs;
    for (auto const& pattern : data.patterns)
    {
        exprs.push_back(pattern.c_str());
    }

    hs_database_t* db = nullptr;
    hs_compile_error_t* compile_err;
    hs_error_t err;
    err = hs_compile_multi(exprs.data(), data.flags.data(), data.ids.data(),
        exprs.size(), mode, nullptr, &db, &compile_err);
    if (err != HS_SUCCESS)
    {
        if (compile_err->expression < 0)
        {
            fprintf(stderr, "[ ERROR ] '%s'\n", compile_err->message);
        }
        else
        {
            fprintf(stderr, "[ ERROR ] Pattern '%s' Failed with error: '%s'\n",
                exprs[compile_err->expression], compile_err->message);
        }
        hs_free_compile_error(compile_err);
        exit(-1);
    }
    return db;
}

// helper functions
void Getline_noCR(istream& is, string& s)
{
    std::getline(is, s);
    if (s.back() == '\r')
    {
        s.pop_back();
    }
}

void SplitString(const string& input, char delim, vector<string>& res)
{
    res.clear();
    string::size_type pos1, pos2;
    size_t input_length = input.size();
    pos2 = input.find(delim);
    pos1 = 0;
    while (pos2 != string::npos)
    {
        res.emplace_back(input.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = input.find(delim, pos1);
    }
    if (pos1 != input_length)
    {
        res.emplace_back(input.substr(pos1));
    }
}

void HsStr2Hex(const char* xstr, char* hexbuf)
{
    for (size_t i = 0; i < strlen(xstr); i++, hexbuf += 4)
    {
        snprintf(hexbuf, 5, "\\x%02x", (unsigned char)xstr[i]);
    }
    *hexbuf = '\0';
}
