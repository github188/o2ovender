#ifndef SD_SMS_CLIENT_H
#define SD_SMS_CLIENT_H

#include <string>
#include <common/SDLogger.h>

extern size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream);

class SDSMSClient
{
public:
    SDSMSClient();
    ~SDSMSClient();

    int send_sms(const std::string& phone_number, const std::string& content);

protected:
    DECL_LOGGER(logger);

    static std::string GetResponseItem(const std::string& strSrc, const std::string& strKey);
};

#endif
