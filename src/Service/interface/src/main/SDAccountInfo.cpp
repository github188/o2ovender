#include "SDAccountInfo.h"

using namespace std;

IMPL_LOGGER(SDMongoAccountInfo, logger);

const string SDMongoAccountInfo::NS = "o2ovender.account_info";
const string SDMongoAccountInfo::FIELD_UID = "_id";
const string SDMongoAccountInfo::FIELD_PASSWD = "passwd";

int SDMongoAccountInfo::query(SDMongoClient* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    //builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    builder.append(FIELD_UID, account_info.m_uid);
    mongo::BSONObj obj;
    int res = mongodb->findone(ns, builder.obj(), &obj);
    LOG4CPLUS_DEBUG(logger, obj.jsonString());
    if (res == 1) {
        if (!obj.hasField(FIELD_PASSWD)) {
            LOG4CPLUS_ERROR(logger, "no passwd field");
            return -1;
        }
        
        mongo::BSONElement e = obj[FIELD_PASSWD];
        account_info.m_passwd = e.String();
        /*int len = 0;
        const char * ptr = e.binData(len);
        account_info.m_passwd.assign(ptr, len);
        */
    }

    return res;
}

int SDMongoAccountInfo::insert(SDMongoClient* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.append(FIELD_UID, account_info.m_uid);
    builder.append(FIELD_PASSWD, account_info.m_passwd);
    //builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    //builder.appendBinData(FIELD_PASSWD, account_info.m_passwd.length(), mongo::BinDataGeneral, account_info.m_passwd.c_str());

    return mongodb->insert(ns, builder.obj());
}
