#ifndef SD_IDENTIFYING_CODE_H
#define SD_IDENTIFYING_CODE_H

#include <string>
#include <stdint.h>

class SDIdentifyingCode
{
public:
    static std::string gen_code(uint64_t id);
};

#endif
