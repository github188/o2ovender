#include "SDAVMRequest.h"

using namespace std;

SDAVMRequest::SDAVMRequest()
{}

SDAVMRequest::~SDAVMRequest()
{}

bool SDAVMRequestencode(char* buf, int* size)
{
    return false;
}

bool SDAVMRequest::decode(const char* buf, int size)
{
    return false;
}

bool SDAVMRequest::encode_head(char* buf, int* size)
{
    return false;
}

bool SDAVMRequest::decode_head(const char* buf, int size)
{
    return false;
}

bool SDAVMRequest::encode_body(char* buf, int* size)
{
    return false;
}

bool SDAVMRequest::decode_body(const char* buf, int size)
{
    return false;
}
