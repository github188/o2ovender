#include "SDStringUtility.h"

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

void SDStringUtility::split(const std::string& s, const std::string & sp, std::vector<std::string>& array)
{
    string::size_type begin = s.empty() ? string::npos : 0;
    string::size_type end = s.find(sp, begin);
    while (begin != string::npos) {
        if (end == string::npos) {
            array.push_back(s.substr(begin));
            begin = end;
        } else {
            array.push_back(s.substr(begin, (end - begin)));
            begin = end;
            if ((begin + sp.length()) >= s.length()) {
                begin = string::npos;
            } else {
                begin += sp.length();
            }
        }
        end = s.find(sp, begin);
    }
}

string SDStringUtility::str2hex(const char * buf,int size)
{
    string ret_str;

    char tmpbuf[10] = {0};
    const char * buffer = buf;
    int buffer_size = size;
    for (int i = 0 ;i < buffer_size; i++) {
        sprintf(tmpbuf,"%02x",(unsigned char)buffer[i]);        
        ret_str += tmpbuf;
    }

    return ret_str;
}

string SDStringUtility::get_local_datestring()
{
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    time_t local_time = (time_t)(tv.tv_sec - tz.tz_minuteswest * 60);
    struct tm lt;
    gmtime_r(&local_time, &lt);

    char timebuf[32];
    sprintf(timebuf, "%04d%02d%02d", (lt.tm_year + 1900), (lt.tm_mon + 1), lt.tm_mday);

    return (string)timebuf;
}

string SDStringUtility::format_time(time_t unix_time)
{
    struct timezone tz;

    gettimeofday(NULL, &tz);

    time_t local_time = (time_t)(unix_time - tz.tz_minuteswest * 60);

    struct tm lt;
    gmtime_r(&local_time, &lt);

    char timebuf[16];
    sprintf(timebuf, "%02d:%02d:%02d", lt.tm_hour, lt.tm_min, lt.tm_sec);

    return (string)timebuf;
}

std::string SDStringUtility::md5_32(const std::string& str)
{
    unsigned char md[MD5_DIGEST_LENGTH] = {0};
    MD5((const unsigned char*)str.c_str(), (unsigned long)str.length(), md);
    return str2hex((const char*)md, MD5_DIGEST_LENGTH);
}
    
std::string SDStringUtility::passive_conn_key_encode(uint32_t index, uint64_t checksum)
{
    ostringstream oss;
    static const char c = ':';
    oss << index << c << checksum;
    return oss.str();
}

void SDStringUtility::passive_conn_key_decode(const std::string& key, uint32_t* index, uint64_t* checksum)
{
    istringstream iss(key);
    char c;
    iss >> (*index) >> c >> (*checksum);
}
