#ifndef SD_CHARACTER_SET_CONVERSION_H
#define SD_CHARACTER_SET_CONVERSION_H

#include <string>

#include <iconv.h>
#include <stdint.h>

class SDCharacterSetConversion
{
public:
	SDCharacterSetConversion(const std::string &to_code, const std::string &from_code);
	~SDCharacterSetConversion();

	std::string Conversion(const std::string &str);
	std::string Conversion(const char *buf, size_t buf_bytes);

protected:
	//std::string mToCode;
	//std::string mFromCode;	

	iconv_t mCD;
};

#endif // SD_CHARACTER_SET_CONVERSION_H

