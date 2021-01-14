#include "worker.h"
#include "serializer.h"
#include "deserializer.h"
// ISO
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
// POSIX
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h> // close

using namespace std;

Worker::Worker()
    : db_(nullptr)
    , dbsize_(0)
{
    for (int i = 0; i < TEST_MAX_SC; i++)
        { sc_[i] = nullptr; }
    printf("Worker Constructed\n");
}

Worker::~Worker()
{
    for (int i = 0; i < TEST_MAX_SC; i++)
    {
        hs_free_scratch(sc_[i]);
    }
    hs_free_database(db_);
}

void Worker::initByCfgFile(const char* filename)
{
    struct HSCollData data;

    parseCfgFile(filename, data);

    db_ = BuildDatabase(data, HS_MODE_BLOCK, "master");
    AllocHSScratch(db_, sc_, TEST_MAX_SC, "txtcfg");
}

void Worker::initBySerializedDB(const char* filename, struct CzyDBInfo* header)
{
    db_ = LoadDatabase(filename, header);
    AllocHSScratch(db_, sc_, TEST_MAX_SC, "SerializedDB");
}

unsigned int Worker::queryDB(const struct TestFlow* flow, int thread_idx)
{
    return ScanDB(flow->data, db_, sc_[thread_idx]);
}


void Worker::parseCfgFile(const char* file, struct HSCollData& data)
{
    printf("Reading file '%s'...\n", file);
    ifstream in_file { file };
    if (!in_file.good()) {
        fprintf(stderr, "Read file '%s' Failed.\n", file);
        return;
    }

    for (int i = 0; !in_file.eof(); i++) {
        string line {};
        getline(in_file, line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        string pcre {};
        int id;
        extractPcreAndId(line, pcre, id);

        data.patterns.push_back(pcre);
        data.flags.push_back(0 | HS_FLAG_SINGLEMATCH);
        data.ids.push_back(id);
    }

    dbsize_ = data.patterns.size();
}


/**
 * @example 1,hello,\w+
*/
void Worker::extractPcreAndId(string line, string& pcre, int& id) {
    vector<string> sp {};
    SplitString(line, ',', sp);
    id = stoi(sp[0]);
    if (!sp[2].empty()) {
        pcre = sp[2];
    } else if (!sp[1].empty()) {
        char buf[2048] = {0};
        HsStr2Hex(sp[1].c_str(), buf);
        pcre = buf;
    } else {
        fprintf(stderr, "Err Expr: '%s'\n", line.c_str());
    }
}

// @UNUSED
bool Worker::saveDatabase(const hs_database_t* db, const char* filename)
{
    printf("Saving database to: '%s' ...\n", filename);

    char* bytes = nullptr;
    size_t len = 0U;
    hs_error_t err = hs_serialize_database(db, &bytes, &len);
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: hs_serialize_database() failed with error %d\n", err);
        return false;
    }

    printf("serialized database's size: %zu\n", len);

    ofstream out(filename, ios::binary);
    out.write(bytes, len);
    out.close();
    free(bytes);

    return true;
}

// @UNUSED
hs_database_t* Worker::loadDatabase(const char* filename)
{
    printf("Loading database from: '%s'.\n", filename);

    char* bytes = nullptr;

    ifstream is;
    is.open(filename, ios::in | ios::binary);
    if (!is.is_open())
    {
        fprintf(stderr, "Open '%s' Failed\n", filename);
        return nullptr;
    }
    is.seekg(0, ios::end);
    size_t len = is.tellg();
    printf("Reading %zu Bytes.\n", len);

    bytes = new char[len];

    is.seekg(0, ios::beg);
    is.read(bytes, len);
    is.close();
    assert(bytes);

    char* info = nullptr;
    hs_error_t err;
    err = hs_serialized_database_info(bytes, len, &info);
    if (err)
    {
        printf("Unable to decode serialized database info: '%d'.\n", err);
    }
    else if (info)
    {
        printf("Serialized database info: '%s'.\n", info);
        free(info);
    }
    else
    {
        printf("Unable to decode serialized database info.\n");
    }

    hs_database_t* db = nullptr;

    err = hs_deserialize_database(bytes, len, &db);

    delete[] bytes;

    if (err != HS_SUCCESS)
    {
        fprintf(stderr, "ERROR: Deserialize database failed with error %d\n", err);
        exit(-1);
    }

    printf("Deserialize Database Successfully.\n");

    return db;
}

// UNUSED
hs_database_t* Worker::loadDBByMmap(const char* filename)
{
    char* bytes = nullptr;
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        return nullptr;
    }

    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        close(fd);
        return nullptr;
    }

    size_t len = st.st_size;

    bytes = (char*) mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
    if (bytes == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed.\n");
        close(fd);
        return nullptr;
    }

    munmap(bytes, len);
    close(fd);

    // use @variable bytes...
}
