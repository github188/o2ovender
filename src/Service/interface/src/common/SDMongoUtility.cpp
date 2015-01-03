#include "SDMongoUtility.h"

using namespace std;

IMPL_LOGGER(SDMongoUtility, logger);

int SDMongoUtility::connect(mongo::DBClientConnection* mongodb, const std::string& host, int port)
{
    int res = -1;
    string errmsg;
   
    try {
        mongodb->connect(mongo::HostAndPort(host, port), errmsg);
   
        LOG4CPLUS_DEBUG(logger, "connect to Mongo " << host << ":" << port << " succ");
        res = 0;
    }
    catch (mongo::DBException& e) {
        LOG4CPLUS_ERROR(logger, "connect to Mongo " << host << ":" << port << " fail: " << e.toString());
        res = -1;
    }

    return res;
}

int SDMongoUtility::insert(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj)
{
    int res = -1;

    try {
        mongodb->insert(ns, obj);
        
        string e = mongodb->getLastError();
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

int SDMongoUtility::findone(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj, mongo::BSONObj* robj)
{
    int res = -1;

    try {
        *robj = mongodb->findOne(ns, obj);

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

int SDMongoUtility::findall(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj, std::vector<mongo::BSONObj>* robj)
{
    int res = -1;

    try {
        std::auto_ptr<mongo::DBClientCursor> cursor = mongodb->query(ns, obj);
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
