#include "SDMobileCommodityRequest.h"

#include <common/SDCharacterSetConversion.h>
#include "SDAccountInfo.h"
#include "SDCommodityInfo.h"
#include "SDSMSClient.h"
#include "SDMongoClient.h"
#include "SDWorkHandler.h"

using namespace std;

IMPL_LOGGER(SDMobileCommodityRequest, logger);

SDMobileCommodityRequest::SDMobileCommodityRequest()
{}

SDMobileCommodityRequest::~SDMobileCommodityRequest()
{}

SDMobileRequest* SDMobileCommodityRequest::clone()
{
    SDMobileCommodityRequest* newobj = new SDMobileCommodityRequest();

    return newobj;
}

int SDMobileCommodityRequest::on_request(std::map<int, void*>& param)
{
    SDMongoClient* mongodb = NULL;
    std::map<int, void*>::iterator it = param.find(SDWorkHandler::MONGODB_CLI);
    if (it!=param.end()) {
        mongodb = (SDMongoClient*)(it->second);
    }
    m_response.set_type(o2ovender::response_TYPE_COMMODITY_LIST);
    o2ovender::commodity_list_resp* commodity_list_resp = m_response.mutable_commodity_list_resp();

    do {
        if (mongodb == NULL) {
            commodity_list_resp->set_result(-1);
            commodity_list_resp->set_msg("server is busy");
            commodity_list_resp->set_errcode(o2ovender::commodity_list_resp_ERRCODE_SERVER_BUSY);
            break;
        }

        std::vector<SDCommodityInfo> commodity_list;
        int res = SDMongoCommodityInfo::list_all(mongodb, commodity_list);
        if (res < 0) {
            commodity_list_resp->set_result(-1);
            commodity_list_resp->set_msg("server is busy");
            commodity_list_resp->set_errcode(o2ovender::commodity_list_resp_ERRCODE_SERVER_BUSY);
            break;
        }
       
        for (unsigned i=0; i<commodity_list.size(); ++i) {
            const SDCommodityInfo& commodity_info = commodity_list[i];
            o2ovender::commodity_info* commodity_info_resp = commodity_list_resp->add_commodity_info();

            commodity_info_resp->set_id(commodity_info.m_id);
            commodity_info_resp->set_type(commodity_info.m_type);
            commodity_info_resp->set_code(commodity_info.m_code);
            commodity_info_resp->set_name(commodity_info.m_name);
            commodity_info_resp->set_img(commodity_info.m_img);
            commodity_info_resp->set_price(commodity_info.m_price);
            commodity_info_resp->set_discount(commodity_info.m_discount);
        }
        commodity_list_resp->set_result(0);
    } while(false);
        
    LOG4CPLUS_DEBUG(logger, "commodity_list_resp\n" << m_response.DebugString());
    
    return send_response();
}
