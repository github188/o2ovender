#ifndef SD_AVM_REQUEST_H
#define SD_AVM_REQUEST_H

#include <string>
#include <stdint.h>
#include <common/SDLogger.h>
#include <common/SDNetFramework.h>

class SDAVMRequest;
typedef boost::shared_ptr<SDAVMRequest> SDSharedAVMRequest;
typedef SDQueue<SDSharedAVMRequest> SDAVMRequestQueue;

class SDAVMRequest
{
public:
    SDAVMRequest();
    virtual ~SDAVMRequest();

    virtual SDAVMRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

    bool encode(char* buf, int* size);
    bool decode(const char* buf, int size);

    bool encode_head(char* buf, int* size);
    bool decode_head(const char* buf, int size);

    virtual bool encode_body(char* buf, int* size);
    virtual bool decode_body(const char* buf, int size);

    void set_timestamp(const char* ptr);
    std::string get_timestamp();
    void set_terminal_number(const char* ptr);
    std::string get_terminal_number();
    void set_version(const char* ptr);
    std::string get_version();
    void set_reserve(const char* ptr);
    std::string get_reserve();
    void set_data_direction(const char* ptr);
    std::string get_data_direction();
    void set_control_code(const char* ptr);
    std::string get_control_code();

public:
    static const int HEAD_SIZE = 37;
    char m_head[HEAD_SIZE];
    
    static std::string SIGN_IN;

    int send_response(SDSharedBuffer& buff);
    SDConnHandler* m_conn_handler;
    std::string m_key;
    uint64_t m_checksum;
};

#endif
