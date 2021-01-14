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

    db_ = ZiBuildDatabase(data, HS_MODE_BLOCK, "master");
    ZiAllocScratchs(db_, sc_, TEST_MAX_SC, "txtcfg");
}

void Worker::initBySerializedDB(const char* filename, bool has_header,
    struct CzyDBInfo* header)
{
    db_ = ZiLoadDatabase(filename, has_header, header);
    ZiAllocScratchs(db_, sc_, TEST_MAX_SC, "SerializedDB");
}

unsigned int Worker::queryDB(const struct TestFlow* flow, int thread_idx)
{
    return ZiScanDB(flow->data, db_, sc_[thread_idx]);
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
