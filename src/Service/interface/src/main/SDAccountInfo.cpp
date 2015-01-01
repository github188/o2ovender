#include "SDAccountInfo.h"

#include <common/SDMongoDBUtility.h>

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
    mongo::BSONObj obj;
    int res = SDMongoDBUtility::findone(mongodb, ns, builder.obj(), &obj);
    LOG4CPLUS_DEBUG(logger, obj.jsonString());
    if (res == 1) {
        if (!obj.hasField(FIELD_PASSWD)) {
            LOG4CPLUS_ERROR(logger, "no passwd field");
            return -1;
        }
        
        mongo::BSONElement e = obj[FIELD_PASSWD];
        int len = 0;
        const char * ptr = e.binData(len);
        account_info.m_passwd.assign(ptr, len);
    }

    return res;
}

int SDMongoDBAccountInfo::insert(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    builder.appendBinData(FIELD_PASSWD, account_info.m_passwd.length(), mongo::BinDataGeneral, account_info.m_passwd.c_str());

    return SDMongoDBUtility::insert(mongodb, ns, builder.obj());
}
