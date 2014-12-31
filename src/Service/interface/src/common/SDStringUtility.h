#ifndef SD_STRING_UTILITY_H
#define SD_STRING_UTILITY_H

#include <string>
#include <vector>

class SDStringUtility
{
public:
    static void split(const std::string& s, const std::string & sp, std::vector<std::string>& array);

    static std::string str2hex(const char* buf, int size);
    static std::string get_local_datestring();
};

#endif
