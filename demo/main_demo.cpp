#include "zi_struct.h"
#include "build_config.h"
#include "serializer.h"
#include "worker.h"

#include <iostream>
#include <cstring>

using namespace std;

static void demo_cfg_file();

static void demo_serialized_db();

int main(int argc, char* argv[])
{
    demo_cfg_file();
    demo_serialized_db();
    return EXIT_SUCCESS;
}

static void demo_cfg_file()
{
    string cfg_dir(DEMO_TXT_CFG_DIR), db_out_dir(DB_OUT_DIR);
    Worker worker;
    worker.initByCfgFile((cfg_dir + "config.cfg").c_str());
    // query
    struct TestFlow flow {"hello0", 5};
    unsigned int id = worker.queryDB(&flow);
    id != 0U ? printf("Query OK, id is '%u'\n", id) : printf("Query Failed.\n");
    // serialize
    // with header
    struct ZiEncryptHdr dbinfo;
    FillHeader(&dbinfo);
    bool serialize_res = ZiSaveDatabase(worker.getDB(), &dbinfo,
                                      (db_out_dir + "out.db").c_str());
    printf("[ Info ] Serialize %s\n", serialize_res ? "Successfully" : "Failed");
    // @TEST without header
    // bool pure_seria_res = ZiSaveDatabase(worker.getDB(), nullptr,
    //                                      (db_out_dir + "pure.db").c_str());
    // printf("[ Info ] Serialize %s\n", pure_seria_res ? "Successfully" : "Failed");
}

static void demo_serialized_db()
{
    string db_dir(DB_IN_DIR);
    Worker worker;
    // with header info
    struct ZiEncryptHdr dbinfo;
    worker.initBySerializedDB((db_dir + "out.db").c_str(), true, &dbinfo);

    printf("[ Info ] Programm Version: %d.%d.%d\n",
        dbinfo.ver_major, dbinfo.ver_minor, dbinfo.ver_patch);

    time_t time = (time_t) dbinfo.btime;
    printf("[ Info ] Build Time: %s\n", ctime(&time));

    // @TEST with header info but don't care.
    // worker.initBySerializedDB((db_dir + "out.db").c_str(), true);
    // @TEST without header info
    // worker.initBySerializedDB((db_dir + "pure.db").c_str(), false);
    // query
    struct TestFlow flow { "hell0", 5 };
    uint id = worker.queryDB(&flow);
    id != 0U ? printf("Query OK, id is '%u'\n", id) : printf("Query Failed.\n");
}
