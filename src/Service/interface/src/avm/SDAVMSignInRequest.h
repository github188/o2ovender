#ifndef SD_AVM_SIGN_IN_REQUEST_H
#define SD_AVM_SIGN_IN_REQUEST_H

#include "SDAVMRequest.h"

class SDAVMSignInRequest : public SDAVMRequest
{
public:
    SDAVMSignInRequest();
    virtual ~SDAVMSignInRequest();

    virtual SDAVMRequest* clone();
    virtual int on_request(std::map<int, void*>& param);

    virtual bool encode_body(char* buf, int* size);
    virtual bool decode_body(const char* buf, int size);

    void set_driver_version(const char* ptr);
    std::string get_driver_version();
    void get_driver_version(char** ptr, int* size);

    void set_soft_version(const char* ptr);
    std::string get_soft_version();
    void get_soft_version(char** ptr, int* size);

    void set_board_version(const char* ptr);
    std::string get_board_version();
    void get_board_version(char** ptr, int* size);

    void set_avm_number(const char* ptr);
    std::string get_avm_number();
    void get_avm_number(char** ptr, int* size);

    void set_machine_type(const char* ptr);
    std::string get_machine_type();
    void get_machine_type(char** ptr, int* size);

    void set_drink_number(const char* ptr);
    std::string get_drink_number();
    void get_drink_number(char** ptr, int* size);

    void set_food_number(const char* ptr);
    std::string get_food_number();
    void get_food_number(char** ptr, int* size);

    void set_noodle_number(const char* ptr);
    std::string get_noodle_number();
    void get_noodle_number(char** ptr, int* size);

    void set_drink_set(const char* ptr);
    std::string get_drink_set();
    void get_drink_set(char** ptr, int* size);

    void set_food_set(const char* ptr);
    std::string get_food_set();
    void get_food_set(char** ptr, int* size);

    void set_noodle_set(const char* ptr);
    std::string get_noodle_set();
    void get_noodle_set(char** ptr, int* size);

public:
    DECL_LOGGER(logger);
    static const int BODY_SIZE = 51;
    char m_body[BODY_SIZE];
};

#endif
