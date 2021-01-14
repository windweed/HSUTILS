#include "deserializer.h"
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

hs_database_t* LoadDatabase(const char* dbfile, struct CzyDBInfo* header)
{
    printf("[ Info ] Loading database from file: '%s'\n", dbfile);

    ifstream is;
    is.open(dbfile, ios::in | ios::binary);
    if (!is.is_open())
    {
        fprintf(stderr, "[ ERROR ] Open '%s' Failed\n", dbfile);
        return nullptr;
    }

    is.seekg(0, ios::end);
    size_t total_len = is.tellg();

    size_t header_size = sizeof(struct CzyDBInfo);
    char* header_buffer = new char[header_size];

    is.seekg(0, ios::beg);
    is.read(header_buffer, header_size);
    if (header)
    {
        *header = *(struct CzyDBInfo*) header_buffer;
    }

    delete[] header_buffer;

    size_t db_size = total_len - header_size;
    char* bytes = nullptr;
    bytes = new char[db_size];
    is.read(bytes, db_size);
    is.close();

    hs_database_t* db;
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
 * @param length 默认值1
*/
void AllocHSScratch(const hs_database_t* db, hs_scratch_t** sc, int length,
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
        if(err != HS_SUCCESS)
        {
            fprintf(stderr, "[ ERROR ] '%s' HS-Clone Failed with: %d\n",
                info, err);
            exit(-1);
        }
    }
}

// hs_scan封装
uint32_t ScanDB(const char* content, const hs_database_t* db, hs_scratch_t* sc)
{
    if (content == nullptr || *content == '\0')
    {
        return 0U;
    }

    uint32_t id = 0U;
    hs_error_t err = hs_scan(db, content, strlen(content), 0, sc, OnMatch, &id);
    if (err != HS_SUCCESS)
    {}
    return id;
}

// hs_scan回调函数。
int OnMatch(unsigned int id, unsigned long long from, unsigned long long to,
    unsigned int flags, void* ctx)
{
    *(uint32_t*)ctx = id;
    return 0;
}
