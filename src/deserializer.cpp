#include "deserializer.h"
#include "zi_struct.h"
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

hs_database_t* ZiLoadDatabase(const char* dbfile, bool has_header,
    struct ZiEncryptHdr* header_recv)
{
    printf("[ Info ] Loading database from file: '%s'\n", dbfile);

    ifstream is;
    is.open(dbfile, ios::in | ios::binary);
    if (!is.is_open())
    {
        fprintf(stderr, "[ ERROR ] Open '%s' Failed\n", dbfile);
        return nullptr;
    }
    // 首先获得文件大小
    is.seekg(0, ios::end);
    size_t total_len = is.tellg();
    // 回到文件头准备读取
    is.seekg(0, ios::beg);
    // 若有头部信息则跳过，可选地提取信息。
    size_t hdr_len = 0;
    if (has_header)
    {
        printf("[ Info ] Deserialize: Extracting header...\n");

        hdr_len = sizeof(struct ZiEncryptHdr); // TODO modify len to magic number
        char* hdr_buffer = new char[hdr_len];
        is.read(hdr_buffer, hdr_len);
        if (header_recv)
        {
            printf("[ Info ] Deserialize: Writing Header...\n");
            *header_recv = *(struct ZiEncryptHdr*) hdr_buffer;
        }
        delete[] hdr_buffer;
    }

    size_t db_size = total_len - hdr_len;
    char* bytes = nullptr;
    bytes = new char[db_size];
    is.read(bytes, db_size);
    is.close();

    hs_database_t* db = nullptr;
    hs_error_t err = hs_deserialize_database(bytes, db_size, &db);

    delete[] bytes;

    if (err != HS_SUCCESS)
    {
        fprintf(stderr, "[ ERROR ] Deserialize Failed with: '%d'\n", err);
        exit(-1);
    }
    printf("[ Info ] Deserialize database successfully\n");

    return db;
}

/**
 * @brief 根据 @p db 为 @p sc 数组(长度由 @p length 指定)分配空间。
 * @param length 默认值1
 * @param info 提示信息
*/
void ZiAllocScratchs(const hs_database_t* db, hs_scratch_t** sc, int length,
    const char* info)
{
    hs_error_t err;
    sc[0] = nullptr;
    err = hs_alloc_scratch(db, &sc[0]);
    if (err != HS_SUCCESS)
    {
        fprintf(stderr, "[ ERROR ] '%s' HS-Alloc Failed with: %d\n", info, err);
        exit(-1);
    }

    for (int i = 1; i < length; i++)
    {
        err = hs_clone_scratch(sc[0], &sc[i]);
        if (err != HS_SUCCESS)
        {
            fprintf(stderr, "[ ERROR ] '%s' HS-Clone Failed with: %d\n",
                info, err);
            exit(-1);
        }
    }
}

uint32_t ZiScanDB(const char* content, const hs_database_t* db, hs_scratch_t* sc)
{
    if (content == nullptr || *content == '\0')
    {
        return 0U;
    }

    uint32_t id = 0U;
    hs_error_t err = hs_scan(db, content, strlen(content), 0, sc, ZiOnMatch, &id);
    if (err != HS_SUCCESS)
    {}
    return id;
}

int ZiOnMatch(unsigned int id, unsigned long long from, unsigned long long to,
    unsigned int flags, void* ctx)
{
    *(uint32_t*)ctx = id;
    return 0;
}
