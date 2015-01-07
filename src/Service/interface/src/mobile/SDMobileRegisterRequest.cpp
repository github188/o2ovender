#include "SDMobileRegisterRequest.h"

#include <common/SDCharacterSetConversion.h>
#include <main/SDAccountInfo.h>
#include <main/SDCommodityInfo.h>
#include <main/SDSMSClient.h>
#include <main/SDRedisClient.h>
#include <main/SDWorkHandler.h>

using namespace std;

IMPL_LOGGER(SDMobileRegisterRequest, logger);

SDMobileRegisterRequest::SDMobileRegisterRequest()
{}

SDMobileRegisterRequest::~SDMobileRegisterRequest()
{}

SDMobileRequest* SDMobileRegisterRequest::clone()
{
    SDMobileRegisterRequest* newobj = new SDMobileRegisterRequest();

    return newobj;
}

int SDMobileRegisterRequest::on_request(std::map<int, void*>& param)
{
    if (!m_request.has_register_req()) {
        LOG4CPLUS_WARN(logger, "No register_req");
        return -1;
    }
    
    SDMongoClient* mongodb = NULL;
    std::map<int, void*>::iterator it = param.find(SDWorkHandler::MONGODB_CLI);
    if (it!=param.end()) {
        mongodb = (SDMongoClient*)(it->second);
    }
    SDRedisClient* redis = NULL;
    it = param.find(SDWorkHandler::REDIS_CLI);
    if (it!=param.end()) {
        redis = (SDRedisClient*)(it->second);
    }
    const o2ovender::register_req& register_req = m_request.register_req();
    const std::string& uid = register_req.uid();
    const std::string& password = register_req.pass_word();
    const std::string& identifying_code = register_req.identifying_code();

    m_response.set_type(o2ovender::response_TYPE_REGISTER);
    o2ovender::register_resp* register_resp = m_response.mutable_register_resp();

    SDAccountInfo account_info;
    account_info.m_uid = uid;
    do {
        if (mongodb == NULL) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        
        if (redis == NULL) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        
        if (uid.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no uid");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_ACCOUNT_EMPTY);
            break;
        }
        
        if (password.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no password");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_PASSWD_EMPTY);
            break;
        }
        
        if (identifying_code.empty()) {
            register_resp->set_result(-1);
            register_resp->set_msg("no identifying code");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_IDCODE_EMPTY);
            break;
        }

        int res = SDMongoAccountInfo::query(mongodb, account_info);
        if (res == -1) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }

        if (res == 1) {
            register_resp->set_result(-1);
            register_resp->set_msg("account exists");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_ACCOUNT_EXISTS);
            break;
        }
           
        string key("code:");
        key += uid;
        string value;
        res = redis->get(key, &value);
        if (res == -1) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        else if (identifying_code != value) {
            register_resp->set_result(-1);
            register_resp->set_msg("ientifying code is wrong");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_IDCODE_WRONG);
            break;
        }
       
        account_info.m_passwd = password;
        res = SDMongoAccountInfo::insert(mongodb, account_info);
        if (res != 1) {
            register_resp->set_result(-1);
            register_resp->set_msg("server is busy");
            register_resp->set_errcode(o2ovender::register_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        
        redis->del(key);
        register_resp->set_result(0);
    } while(false);
        
    LOG4CPLUS_DEBUG(logger, "register_resp\n" << m_response.DebugString());

    return send_response();
}
