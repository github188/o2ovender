#include "SDCharacterSetConversion.h"

using namespace std;

SDCharacterSetConversion::
	SDCharacterSetConversion(const string &to_code, const string &from_code)
{	
	//mToCode = to_code;
	//mFromCode = from_code;

	mCD = iconv_open( to_code.c_str(), from_code.c_str() );
}

SDCharacterSetConversion::
	~SDCharacterSetConversion()
{
	iconv_close( mCD );
}

string SDCharacterSetConversion::
	Conversion(const char *buf, size_t buf_bytes)
{
	static const uint32_t STACK_SIZE = 4096; /* 4KB */	
	char stack_buffer[ STACK_SIZE ];

	uint32_t need_bytes = (uint32_t)((double)(buf_bytes) * 1.5);
	char *pbuf = NULL;
	
	string out;

	if ( need_bytes > STACK_SIZE ) {
		pbuf = new char[ need_bytes ];		
	} else {
		pbuf = stack_buffer;
	}

	char* inbuf = (char*)buf;
	char* outbuf = pbuf;
	size_t inbytesleft = buf_bytes;
	size_t outbytesleft = need_bytes;

	if( iconv( mCD, &inbuf, &inbytesleft, &outbuf, &outbytesleft ) != (size_t)(-1) ) { // errno=E2BIG outbuf not enough!!!
		out.append( pbuf, (need_bytes - outbytesleft) );
	} else {
		out.append( buf, buf_bytes );
	}

	if ( need_bytes > STACK_SIZE ) {
		delete[] pbuf;
	}

	return out;
}

string SDCharacterSetConversion::
	Conversion(const string& str)
{
	return Conversion( (char*)str.c_str(), str.length() );
}

