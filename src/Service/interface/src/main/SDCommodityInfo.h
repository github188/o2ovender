#ifndef SD_COMMODITY_INFO_H
#define SD_COMMODITY_INFO_H

#include <string>
#include <vector>
#include <common/SDLogger.h>
#include <client/dbclient.h>

class SDCommodityInfo
{
public:
    std::string m_id;
    int m_type;
    int m_code;
    std::string m_name;
    std::string m_img;
    double m_price;
    double m_discount;
};

class SDMongoCommodityInfo
{
public:
    static int list_all(mongo::DBClientConnection* mongodb, std::vector<SDCommodityInfo>& commodity_list);
    
    static int query(mongo::DBClientConnection* mongodb, SDCommodityInfo& commodity_info);
    static int insert(mongo::DBClientConnection* mongodb, SDCommodityInfo& commodity_info);

private:
    static const std::string NS;

    static const std::string FIELD_ID;
    static const std::string FIELD_TYPE;
    static const std::string FIELD_CODE;
    static const std::string FIELD_NAME;
    static const std::string FIELD_IMG;
    static const std::string FIELD_PRICE;
    static const std::string FIELD_DISCOUNT;

protected:
    DECL_LOGGER(logger);
};

#endif
