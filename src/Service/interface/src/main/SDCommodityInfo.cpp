#include "SDCommodityInfo.h"

#include <common/SDMongoUtility.h>

using namespace std;

IMPL_LOGGER(SDMongoCommodityInfo, logger);

const string SDMongoCommodityInfo::NS = "o2ovender.commodity_info";
const string SDMongoCommodityInfo::FIELD_ID = "_id";
const string SDMongoCommodityInfo::FIELD_TYPE = "type";
const string SDMongoCommodityInfo::FIELD_CODE = "code";
const string SDMongoCommodityInfo::FIELD_NAME = "name";
const string SDMongoCommodityInfo::FIELD_IMG = "img";
const string SDMongoCommodityInfo::FIELD_PRICE = "price";
const string SDMongoCommodityInfo::FIELD_DISCOUNT = "discount";
    
int SDMongoCommodityInfo::list_all(mongo::DBClientConnection* mongodb, std::vector<SDCommodityInfo>& commodity_list)
{
    const string& ns = NS;
    //builder.appendBinData(FIELD_UID, commodity_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    //mongo::BSONObj obj;
    std::vector<mongo::BSONObj> obj_list;
    int res = SDMongoUtility::findall(mongodb, ns, mongo::BSONObj(), &obj_list);
    //LOG4CPLUS_DEBUG(logger, obj.jsonString());
    if (res > 0) {
        for (unsigned i=0; i<obj_list.size(); ++i) {
            SDCommodityInfo commodity_info;
            const mongo::BSONObj& obj = obj_list[i];
            
            if (obj.hasField(FIELD_ID)) {
                commodity_info.m_id = obj[FIELD_ID].String();
                /*
                mongo::BSONElement e = obj[FIELD_ID];
                int len = 0;
                const char * ptr = e.binData(len);
                commodity_info.m_id.assign(ptr, len);
                */
            }
            if (obj.hasField(FIELD_TYPE)) {
                mongo::BSONElement e = obj[FIELD_TYPE];
                if (e.type() == mongo::NumberInt) {
                    commodity_info.m_type = e.Int();
                }
                else if (e.type() == mongo::NumberLong) {
                    commodity_info.m_type = e.Long();
                }
                else
                    commodity_info.m_type = 0;
            }
            else {
                commodity_info.m_type = 0;
            }
            if (obj.hasField(FIELD_CODE)) {
                mongo::BSONElement e = obj[FIELD_CODE];
                if (e.type() == mongo::NumberInt) {
                    commodity_info.m_code = e.Int();
                }
                else if (e.type() == mongo::NumberLong) {
                    commodity_info.m_code = e.Long();
                }
                else
                    commodity_info.m_code = 0;
            }
            else {
                commodity_info.m_type = 0;
            }
            if (obj.hasField(FIELD_NAME)) {
                commodity_info.m_name = obj[FIELD_NAME].String();
            }
            if (obj.hasField(FIELD_IMG)) {
                commodity_info.m_img = obj[FIELD_IMG].String();
            }
            if (obj.hasField(FIELD_PRICE)) {
                mongo::BSONElement e = obj[FIELD_PRICE];
                if (e.type() == mongo::NumberInt) {
                    commodity_info.m_price = e.Int();
                }
                else if (e.type() == mongo::NumberLong) {
                    commodity_info.m_price = e.Long();
                }
                else
                    commodity_info.m_price = obj[FIELD_PRICE].Double();
            }
            else {
                commodity_info.m_price = 0.0;
            }
            if (obj.hasField(FIELD_DISCOUNT)) {
                commodity_info.m_discount = obj[FIELD_DISCOUNT].Double();
            }
            else {
                commodity_info.m_discount = 0.0;
            }

            commodity_list.push_back(commodity_info);
        }
    }

    return res;
}

int SDMongoCommodityInfo::query(mongo::DBClientConnection* mongodb, SDCommodityInfo& commodity_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    //builder.appendBinData(FIELD_UID, commodity_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    builder.append(FIELD_ID, commodity_info.m_id);
    mongo::BSONObj obj;
    int res = SDMongoUtility::findone(mongodb, ns, builder.obj(), &obj);
    LOG4CPLUS_DEBUG(logger, obj.jsonString());
    if (res == 1) {
        //mongo::BSONElement e = obj[FIELD_PASSWD];
        //commodity_info.m_passwd = e.String();
        /*int len = 0;
        const char * ptr = e.binData(len);
        commodity_info.m_passwd.assign(ptr, len);
        */
    }

    return res;
}

int SDMongoCommodityInfo::insert(mongo::DBClientConnection* mongodb, SDCommodityInfo& commodity_info)
{
    const string& ns = NS;
    mongo::BSONObjBuilder builder;
    builder.append(FIELD_ID, commodity_info.m_id);
    //builder.appendBinData(FIELD_UID, commodity_info.m_uid.length(), mongo::BinDataGeneral, account_info.m_uid.c_str());
    //builder.appendBinData(FIELD_PASSWD, commodity_info.m_passwd.length(), mongo::BinDataGeneral, account_info.m_passwd.c_str());

    return SDMongoUtility::insert(mongodb, ns, builder.obj());
}
