#ifndef _DEMO_WORKER_H__
#define _DEMO_WORKER_H__

#include <vector>
#include <string>
#include <cstdint>
#include "hs.h"

#define TEST_MAX_SC 10 // assumed amount of threads

// test strings
struct TestFlow {
    const char* data;
    int data_len;
};


class Worker {
private:
    hs_database_t* db_;
    hs_scratch_t*  sc_[TEST_MAX_SC];
    size_t dbsize_; // amount of the rules to be compiled.

public:
    Worker();
    ~Worker();

    void initByCfgFile(const char* file);
    void initBySerializedDB(const char* file, bool has_header,
        struct ZiEncryptHdr* dinf = nullptr);

    hs_database_t* getDB() const { return db_; }
    size_t getDBsize() const { return dbsize_; }

    unsigned int queryDB(const struct TestFlow* flow, int thread_idx = 0);

private:
    // @UNUSED original deserialize interface。 using mmap
    hs_database_t* loadDBByMmap(const char* filename);

    void parseCfgFile(const char* file, struct ZiHSCollData& data);
    // helper
    void extractPcreAndId(std::string line, std::string& pcre, int& id);
};

#endif /* _DEMO_WORKER_H__ */
