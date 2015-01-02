#include "SDAccountInfo.h"

#include <common/SDMongoUtility.h>

using namespace std;

IMPL_LOGGER(SDMongoAccountInfo, logger);

const string SDMongoAccountInfo::NS = "o2ovender.account_info";
const string SDMongoAccountInfo::FIELD_UID = "_id";
const string SDMongoAccountInfo::FIELD_PASSWD = "passwd";

int SDMongoAccountInfo::query(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    mongo::BSONObj obj;
    int res = SDMongoUtility::findone(mongodb, ns, builder.obj(), &obj);
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

int SDMongoAccountInfo::insert(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.appendBinData(FIELD_UID, account_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    builder.appendBinData(FIELD_PASSWD, account_info.m_passwd.length(), mongo::BinDataGeneral, account_info.m_passwd.c_str());

    return SDMongoUtility::insert(mongodb, ns, builder.obj());
}
