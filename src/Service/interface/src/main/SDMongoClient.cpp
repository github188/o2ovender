#include "SDMongoClient.h"

using namespace std;

IMPL_LOGGER(SDMongoClient, logger);

SDMongoClient::SDMongoClient() : m_mongodb(true)
{}

int SDMongoClient::connect( const std::string& host, int port)
{
    int res = -1;
    string errmsg;
   
    try {
        m_mongodb.connect(mongo::HostAndPort(host, port), errmsg);
   
        LOG4CPLUS_DEBUG(logger, "connect to Mongo " << host << ":" << port << " succ");
        res = 0;
    }
    catch (mongo::DBException& e) {
        LOG4CPLUS_ERROR(logger, "connect to Mongo " << host << ":" << port << " fail: " << e.toString());
        res = -1;
    }

    return res;
}

int SDMongoClient::insert(const std::string& ns, const mongo::BSONObj& obj)
{
    int res = -1;

    try {
        m_mongodb.insert(ns, obj);
        
        string e = m_mongodb.getLastError();
        if ( !e.empty() ) {
            LOG4CPLUS_ERROR(logger, "insert Mongo fail: " << e);
            res = -1;
        }
        else {
            LOG4CPLUS_DEBUG(logger, "insert Mongo succ");
            res = 1;
        }
    }
    catch (const mongo::OperationException &) {
        LOG4CPLUS_ERROR(logger, "insert Mongo fail: " << "duplicate key error");
        res = 0;
    }

    return res;
}

int SDMongoClient::findone(const std::string& ns, const mongo::BSONObj& obj, mongo::BSONObj* robj)
{
    int res = -1;

    try {
        *robj = m_mongodb.findOne(ns, obj);

        if (robj->isEmpty()) {
            LOG4CPLUS_DEBUG(logger, "query Mongo succ: " << "empty set");
            res = 0;
        }
        else {
            res = 1;
        }
    }
    catch (mongo::DBException& e){
        LOG4CPLUS_ERROR(logger, "query Mongo fail: " << e.toString());
        res = -1;
    }

    return res;
}

int SDMongoClient::findall(const std::string& ns, const mongo::BSONObj& obj, std::vector<mongo::BSONObj>* robj)
{
    int res = -1;

    try {
        std::auto_ptr<mongo::DBClientCursor> cursor = m_mongodb.query(ns, obj);
        if (!cursor.get()) {
            LOG4CPLUS_DEBUG(logger, "query Mongo fail");
            res = -1;
        }

        while (cursor->more()) {
            robj->push_back(cursor->next());
        }
        res = robj->size();
        LOG4CPLUS_DEBUG(logger, "query Mongo succ: total " << res << " records");
    }
    catch (mongo::DBException& e){
        LOG4CPLUS_ERROR(logger, "query Mongo fail: " << e.toString());
        res = -1;
    }

    return res;
}
