#ifndef SANDAI_CPP_SDZLIB_H_20061107
#define SANDAI_CPP_SDZLIB_H_20061107

#include "zlib.h"

class SDZLIB
{
public:
	static bool compress_buffer(char* dest, uint64_t* dest_len, const char* source, uint64_t source_len)
	{
		return (Z_OK == 
				compress2((Bytef*)dest, (uLongf*)dest_len, (const Bytef*)source, (uLongf)source_len, Z_DEFAULT_COMPRESSION));
	}
	static bool uncompress_buffer(char *dest, uint64_t* dest_len, const char* source, uint64_t source_len)
	{
		return (Z_OK ==
				uncompress((Bytef*)dest, (uLongf*)dest_len, (const Bytef*)source, (uLongf)source_len));
	}
};

#endif // SANDAI_CPP_SDZLIB_H_20061107

