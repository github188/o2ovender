#include "SDMobileCodeRequest.h"

#include <common/SDCharacterSetConversion.h>
#include "SDAccountInfo.h"
#include "SDCommodityInfo.h"
#include "SDSMSClient.h"
#include "SDRedisClient.h"
#include "SDWorkHandler.h"

using namespace std;

IMPL_LOGGER(SDMobileCodeRequest, logger);

SDMobileCodeRequest::SDMobileCodeRequest()
{}

SDMobileCodeRequest::~SDMobileCodeRequest()
{}

SDMobileRequest* SDMobileCodeRequest::clone()
{
    SDMobileCodeRequest* newobj = new SDMobileCodeRequest();

    return newobj;
}

int SDMobileCodeRequest::on_request(std::map<int, void*>& param)
{
    SDRedisClient* redis = NULL;
    std::map<int, void*>::iterator it = param.find(SDWorkHandler::REDIS_CLI);
    if (it!=param.end()) {
        redis = (SDRedisClient*)(it->second);
    }
    const o2ovender::get_identifying_code_req& get_identifying_code_req = m_request.get_identifying_code_req();
    const std::string& uid = get_identifying_code_req.uid();

    char buf[1024] = {'\0'};
    do {
        if (redis == NULL) {
            LOG4CPLUS_DEBUG(logger, "server is busy");
            break;
        }
        
        if (uid.empty()) {
            LOG4CPLUS_DEBUG(logger, "no uid");
            break;
        }
        
        string key("code:");
        key += uid;
        string code;
        int res = redis->get(key, &code);
        if (res == -1) {
            break;
        }
      
        if (code.empty()) {
            uint32_t c = (uint32_t)(m_checksum%1000000);
            sprintf(buf, "%06u", c);
            code = buf;
            LOG4CPLUS_DEBUG(logger, "new code: " << code);

            res = redis->set(key, code, 1800);
            if (res != 0) {
                break;
            }
        }
        else {
            LOG4CPLUS_DEBUG(logger, "old code: " << code);
        }
           
        //SDCharacterSetConversion conv("gbk", "utf8");
        sprintf(buf, "注册验证码：%s，三十分钟内有效，请尽快完成注册。【便利超人】", code.c_str());
        //string content = conv.Conversion(buf, strlen(buf));
        string content(buf);
        LOG4CPLUS_DEBUG(logger, "sms: " << content);

        SDSMSClient sms_cli;
        res = sms_cli.send_sms(uid, content);

    } while(false);
    
    return 0;
}
