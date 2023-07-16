#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"
#include "rocksdb/rocksdb_namespace.h"

#if defined(OS_WIN)
std::string kDBPath = "C:\\Windows\\TEMP\\rocksdb";
#else
std::string kDBPath = "/root/temp/rocks_test";
#endif

using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;
using ROCKSDB_NAMESPACE::ColumnFamilyOptions;
using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::DBOptions;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Slice;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

using VectorTuple = std::vector<std::tuple<std::string, std::string>>;
using VectorString = std::vector<std::string>;

std::string getAfterDelim(char delim,std::string input_string);

std::string getBeforeDelim(char delim,std::string input_string);

class RocksDB{
    private:
        DB* db;
        Options options;
        Status s;
        std::vector<ColumnFamilyDescriptor> instances;
        std::vector<ColumnFamilyHandle*> instance_handlers;

    public:
        RocksDB();

        // UTILS
        ColumnFamilyHandle* getHandlerByName(std::string name);
        ColumnFamilyHandle* createInstance(std::string name);

        // CREATE
        bool add(ColumnFamilyHandle* handler,std::string key,std::string value);

        // READ
        std::string get(ColumnFamilyHandle* handler,std::string keyval);

        VectorTuple getByPrefix(ColumnFamilyHandle* handler,std::string prefix);
        
        VectorTuple getBySuffix(ColumnFamilyHandle* handler,std::string suffix);

        VectorTuple scan(ColumnFamilyHandle* handler);
        
        VectorTuple scanConditional(ColumnFamilyHandle* handler,std::string key,std::string condition);

        // UPDATE
        Status update(ColumnFamilyHandle* handler,std::string key,std::string value);

        // DELETE 
        Status singleDelete(ColumnFamilyHandle* handler,std::string key);

        ~RocksDB();
};
