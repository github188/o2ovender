#include "SDAccountInfo.h"

using namespace std;

IMPL_LOGGER(SDMongoDBAccountInfo, logger);

const string SDMongoDBAccountInfo::NS = "o2ovender.account_info";
const string SDMongoDBAccountInfo::FIELD_UID = "_id";
const string SDMongoDBAccountInfo::FIELD_PASSWD = "passwd";

int SDMongoDBAccountInfo::query(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    try {
        auto_ptr<mongo::DBClientCursor> cursor = mongodb->query(ns, builder.obj());
        if (cursor.get() == NULL) {
            LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << "cursor = NULL");
            return -1;
        }
        if (!cursor->more()) {
            LOG4CPLUS_ERROR(logger, "query MongoDB succ: " << "empty set");
            return 0;
        }
           
        mongo::BSONObj obj = cursor->next();
        if(!mongo::getErrField(obj).eoo() ) {
            LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << obj.jsonString());
            return -1;
        }

        if (!obj.hasField(FIELD_PASSWD)) {
            LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << "no passwd field");
            return -1;
        }
        
        mongo::BSONElement e = obj[FIELD_PASSWD];
        int len = 0;
        const char * ptr = e.binData(len);
        account_info.m_passwd.assign(ptr, len);
        LOG4CPLUS_DEBUG(logger, "query MongoDB succ: " << obj.jsonString());
        return 1;
    }
    catch (mongo::DBException& e){
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << e.toString());
    }
    catch (std::exception& e) {
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << e.what());
    }
    catch(...) {
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << "Unknown Exception");
    }

    return -1;
}

int SDMongoDBAccountInfo::insert(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    builder.appendBinData(FIELD_PASSWD, account_info.m_passwd.length(), mongo::BinDataGeneral, account_info.m_passwd.c_str());
    try {
        mongodb->insert(ns, builder.obj());
        //mongo::BSONObj errmsg = mongodb->getLastErrorDetailed();
        //if (errmsg["n"].numberInt() > 0) { //  errmsg.jsonString()
        string errmsg = mongodb->getLastError();
        if (errmsg.empty()) {
            LOG4CPLUS_DEBUG(logger, "insert MongoDB succ: " << errmsg);
            return 1;
        }
        else {
            LOG4CPLUS_ERROR(logger, "insert MongoDB fail: " << errmsg);
            return -1;
        }
    }
    catch (mongo::DBException& e){
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << e.toString());
    }
    catch (std::exception& e) {
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << e.what());
    }
    catch(...) {
        LOG4CPLUS_ERROR(logger, "query MongoDB fail: " << "Unknown Exception");
    }

    return -1;
}
