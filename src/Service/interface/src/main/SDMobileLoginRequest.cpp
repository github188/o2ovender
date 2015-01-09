#include "SDMobileLoginRequest.h"

#include <common/SDCharacterSetConversion.h>
#include "SDAccountInfo.h"
#include "SDCommodityInfo.h"
#include "SDSMSClient.h"
#include "SDMongoClient.h"
#include "SDWorkHandler.h"

using namespace std;

IMPL_LOGGER(SDMobileLoginRequest, logger);

SDMobileLoginRequest::SDMobileLoginRequest()
{}

SDMobileLoginRequest::~SDMobileLoginRequest()
{}

SDMobileRequest* SDMobileLoginRequest::clone()
{
    SDMobileLoginRequest* newobj = new SDMobileLoginRequest();

    return newobj;
}

int SDMobileLoginRequest::on_request(std::map<int, void*>& param)
{
    SDMongoClient* mongodb = NULL;
    std::map<int, void*>::iterator it = param.find(SDWorkHandler::MONGODB_CLI);
    if (it!=param.end()) {
        mongodb = (SDMongoClient*)(it->second);
    }
    const o2ovender::login_req& login_req = m_request.login_req();
    const std::string& uid = login_req.uid();
    const std::string& password = login_req.pass_word();

    m_response.set_type(o2ovender::response_TYPE_LOGIN);
    o2ovender::login_resp* login_resp = m_response.mutable_login_resp();

    SDAccountInfo account_info;
    account_info.m_uid = uid;
    do {
        if (mongodb == NULL) {
            login_resp->set_result(-1);
            login_resp->set_msg("server is busy");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_SERVER_BUSY);
            break;
        }
        
        if (uid.empty()) {
            login_resp->set_result(-1);
            login_resp->set_msg("no uid");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_ACCOUNT_EMPTY);
            break;
        }
        
        if (password.empty()) {
            login_resp->set_result(-1);
            login_resp->set_msg("no password");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_PASSWD_EMPTY);
            break;
        }
       
        int res = SDMongoAccountInfo::query(mongodb, account_info);
        if (res == -1) {
            login_resp->set_result(-1);
            login_resp->set_msg("server is busy");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_SERVER_BUSY);
            break;
        }

        if (res == 0) {
            login_resp->set_result(-1);
            login_resp->set_msg("account not exists");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_ACCOUNT_NOT_EXISTS);
            break;
        }
        
        if (password != account_info.m_passwd) {
            login_resp->set_result(-1);
            login_resp->set_msg("password is wrong");
            login_resp->set_errcode(o2ovender::login_resp_ERRCODE_PASSWD_WRONG);
            break;
        }
            
        login_resp->set_result(0);
    } while(false);
        
    LOG4CPLUS_DEBUG(logger, "login_resp\n" << m_response.DebugString());
   
    return send_response();
}
