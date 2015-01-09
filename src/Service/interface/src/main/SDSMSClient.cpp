#include "SDSMSClient.h"
#include <curl/curl.h>
#include <stdlib.h>

using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
    size_t realsize = size * nmemb;
    std::string temp(static_cast<const char*>(ptr), realsize);
    stream.append(temp);

    return realsize;
}

IMPL_LOGGER(SDSMSClient, logger);

SDSMSClient::SDSMSClient()
{}

SDSMSClient::~SDSMSClient()
{}

int SDSMSClient::send_sms(const std::string& phone_number, const std::string& content)
{
    bool status = false;
    char req_url[1024];
    sprintf(req_url, "http://api.sms.cn/mt/?uid=hulo&pwd=65b43b6de1c63b4dd4a5dbb9c27903b5&encode=utf8&mobile=%s&content=%s", phone_number.c_str(), content.c_str());
    LOG4CPLUS_DEBUG(logger, "HTTP request: " << req_url);

    string response;
    CURL *curl = NULL;
    do {
        curl = curl_easy_init();
        if (!curl) {
            LOG4CPLUS_WARN(logger, "curl_easy_init() fail");
            break;
        }

        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L); 
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_URL, req_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);       

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            LOG4CPLUS_WARN(logger, "curl_easy_perform() fail: " << curl_easy_strerror(res));
            break;
        }
        LOG4CPLUS_DEBUG(logger, "HTTP response: " << response);

        status = true;
    } while (false);

    if (curl) {
        curl_easy_cleanup(curl);
    }

    if (status) {
        int stat = atoi(GetResponseItem(response, "stat").c_str());
        if (stat == 100) {
            return 0;
        }
        //GetResponseItem(response, "message");
    }
    
    return -1;
}

string SDSMSClient::GetResponseItem(const  string & strSrc,const std :: string & strKey)
{
    string strValue;
    string::size_type pos = strSrc.find(strKey);
    if (pos != string::npos) {
        for (pos += strKey.size(); pos < strSrc.size(); ++pos) {
            char c = strSrc[pos];
            if (c == '&')
                break;
            if (c != '=' && !isspace(c))
                strValue.append(1, c);
        }       
    }
    return strValue;
}
