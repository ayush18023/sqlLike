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

std::string getAfterDelim(char delim,std::string input_string){
    return input_string.substr(input_string.find(delim) + 1);
}

std::string getBeforeDelim(char delim,std::string input_string){
    return input_string.substr(0,input_string.find(delim));
}

class RocksDB{
    private:
        DB* db;
        Options options;
        Status s;
        std::vector<ColumnFamilyDescriptor> instances;
        std::vector<ColumnFamilyHandle*> instance_handlers;

    public:
        RocksDB(){        
            options.create_if_missing = true;
            std::vector<std::string> column_families;
            s = DB::ListColumnFamilies(options, kDBPath,&column_families);
            for (const auto& cf : column_families) {
                std::cout << "Column Family: " << cf << std::endl;
                instances.push_back(ColumnFamilyDescriptor(cf, ColumnFamilyOptions()));
            }
            s = DB::Open(DBOptions(), kDBPath, instances, &instance_handlers, &db);
        }

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

        ~RocksDB(){
            std::cout<<"deleted"<<std::endl;
            for(auto handle:instance_handlers){
                delete handle;
            }
            delete db;
        }
};

ColumnFamilyHandle* RocksDB::getHandlerByName (std::string name){
    for (const auto& handle : instance_handlers) {
        if (name == handle->GetName()) {
            return handle;
        }
    }
    return nullptr;
}

ColumnFamilyHandle* RocksDB::createInstance(std::string name){
    ColumnFamilyHandle* cf;
    ColumnFamilyOptions cfOptions;
    s = db->CreateColumnFamily(cfOptions,name,&cf);
    assert(s.ok());

    //delete 
    for(auto handle:instance_handlers){
        delete handle;
    }
    delete db;

    //reopen
    options.create_if_missing = true;
    std::vector<std::string> column_families;
    s = DB::ListColumnFamilies(options, kDBPath,&column_families);
    for (const auto& cf : column_families) {
        std::cout << "Column Family: " << cf << std::endl;
        instances.push_back(ColumnFamilyDescriptor(cf, ColumnFamilyOptions()));
    }
    s = DB::Open(DBOptions(), kDBPath, instances, &instance_handlers, &db);
    
    return cf;
}

bool RocksDB::add(ColumnFamilyHandle* handler,std::string key,std::string value){
    std::string resp = this->get(handler,key);
    if(resp==""){
        s = db->Put(WriteOptions(),handler,key,value);
        if(s.ok()){
            return true;
        }
    }
    return false;
}


std::string RocksDB::get(ColumnFamilyHandle* handler,std::string keyval){
    std::string value;
    s = db->Get(rocksdb::ReadOptions(),keyval,&value);
    if(s.ok()){
        return value;
    }
    else{
        return "";
    }
}

VectorTuple RocksDB::getByPrefix(ColumnFamilyHandle* handler,std::string prefix){
    const Slice key = Slice(prefix);
    VectorTuple response;
    rocksdb::Iterator* it = db->NewIterator(ReadOptions(), handler);
    for(it->Seek(key); it->Valid() && it->key().starts_with(key); it->Next()){
        response.emplace_back(
            std::make_pair(getAfterDelim('#',it->key().ToString()),it->value().ToString())
        );
    }
    return response;
}


VectorTuple RocksDB::getBySuffix(ColumnFamilyHandle* handler,std::string suffix){
    const Slice key = Slice(suffix);
    VectorTuple response;
    rocksdb::Iterator* it = db->NewIterator(ReadOptions(), handler);
    for(it->SeekToFirst(); it->Valid() && it->key().ends_with(key); it->Next()){
        response.emplace_back(
            std::make_pair(getAfterDelim('#',it->key().ToString()),it->value().ToString())
        );
    }
    return response;
}


VectorTuple RocksDB::scan(ColumnFamilyHandle* handler){
    VectorTuple response;
    rocksdb::Iterator* it = db->NewIterator(ReadOptions(), handler);
    for(it->SeekToFirst(); it->Valid(); it->Next()){
        response.emplace_back(
            std::make_pair(it->key().ToString(),it->value().ToString())
        );
    }
    return response;
}

VectorTuple RocksDB::scanConditional(ColumnFamilyHandle* handler,std::string condition_key,std::string condition_value){
    const Slice key = Slice(condition_key);
    VectorTuple response;
    rocksdb::Iterator* it = db->NewIterator(ReadOptions(), handler);
    for(it->SeekToFirst(); it->Valid(); it->Next()){
        if(it->key().ends_with(key) && it->value().ToString() == Slice(condition_value)){
            response.emplace_back(
                std::make_pair(it->key().ToString(),it->value().ToString())
            );
        }
    }
    return response;
}

Status RocksDB::update(ColumnFamilyHandle* handler,std::string key,std::string value){
    s = db->Put(WriteOptions(),handler,key,value);
    return s;
}

Status RocksDB::singleDelete(ColumnFamilyHandle* handler,std::string key){
    s = db->SingleDelete(rocksdb::WriteOptions(), key);
    return s;
}


// int main(){
//     RocksDB rb;

//     ColumnFamilyHandle* cfhandler = rb.createInstance(".config");
//     ColumnFamilyHandle* cf = rb.getHandlerByName("new_ccc");

//     std::cout<< cf->GetName() << std::endl;
//     VectorTuple v = {
//         {"score","420"},
//         {"name","69"}
//     };

//     Status s = rb.update(cf,"John","doe");
//     assert(s.ok());
//     std::string value = rb.get(cf,"juan");
//     std::cout << value <<std::endl;

//     VectorTuple response = rb.scan(cf);
//     for(auto res:response){
//         std::cout << std::get<0>(res) << " = " << std::get<1>(res)<<std::endl; 
//     }

//     return 0;
// }