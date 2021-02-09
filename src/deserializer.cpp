#include "deserializer.h"
#include "zi_struct.h"
#include <fstream>
#include <iostream>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;
using std::ios;

hs_database_t* ZiLoadDatabase(const char* dbfile, bool has_header,
    struct ZiEncryptHdr* header_recver)
{
    cout << "[ Info ] Loading database from file: " << dbfile << endl;

    std::ifstream is;
    is.open(dbfile, ios::in | ios::binary);
    if (!is.is_open())
    {
        cerr << "[ ERROR ]Open " << dbfile << " Failed" << endl;
        return nullptr;
    }
    // Get the file's size
    is.seekg(0, ios::end);
    size_t total_len = is.tellg();
    // back to beginning and ready to read.
    is.seekg(0, ios::beg);
    // if header exists, extrace it first if @p header_recver is not nullptr
    size_t hdr_len = 0;
    if (has_header)
    {
        cout << "[ Info ] Deserialize: Extracting header..." << endl;

        hdr_len = sizeof(struct ZiEncryptHdr); // TODO modify len to magic number
        char* hdr_buffer = new char[hdr_len];
        is.read(hdr_buffer, hdr_len);
        if (header_recver)
        {
            cout << "[ Info ] Deserialize: Writing Header..." << endl;
            *header_recver = *(struct ZiEncryptHdr*) hdr_buffer;
        }
        delete[] hdr_buffer;
    }
    // now read the real db data.
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
        cerr << "[ ERROR ] Deserialize Failed with: " << err << endl;
        exit(-1);
    }
    printf("[ Info ] Deserialize database successfully\n");

    return db;
}

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
