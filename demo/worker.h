#ifndef _DEMO_WORKER_H__
#define _DEMO_WORKER_H__

#include "structs.h"
#include <vector>
#include <string>
#include <cstdint>
#include "hs.h"

#define TEST_MAX_SC 10 // 假设的线程数目

// 用作测试的Flow字符串
struct TestFlow {
    const char* data;
    int data_len;
};


class Worker {
private:
    hs_database_t* db_;
    hs_scratch_t*  sc_[TEST_MAX_SC];
    size_t dbsize_; // 规则数目

public:
    Worker();
    ~Worker();

    void initByCfgFile(const char* file);
    void initBySerializedDB(const char* file, struct CzyDBInfo* dinf = nullptr);

    // @UNUSED 废弃 原始的序列化接口
    bool saveDatabase(const hs_database_t* db, const char* filename);

    hs_database_t* getDB() const { return db_; }
    size_t getDBsize() const { return dbsize_; }

    unsigned int queryDB(const struct TestFlow* flow, int thread_idx = 0);

private:
    // @UNUSED 废弃 原始的反序列化接口。 使用std::ifstream
    hs_database_t* loadDatabase(const char* filename);
    // @UNUSED 废弃 原始的反序列化接口。 使用mmap
    hs_database_t* loadDBByMmap(const char* filename);

    void parseCfgFile(const char* file, struct HSCollData& data);
    // helper
    void extractPcreAndId(std::string line, std::string& pcre, int& id);
};

#endif /* _DEMO_WORKER_H__ */
