#!/bin/sh

echo "#ifndef $HEAD_MACRO"
echo "#define $HEAD_MACRO"
echo
echo "#include \"SDAVMRequest.h\""
echo 
echo "class $CLASS_NAME : public SDAVMRequest"
echo "{"
echo "public:"
echo "    ${CLASS_NAME}();"
echo "    virtual ~${CLASS_NAME}();"
echo
echo "    virtual SDAVMRequest* clone();"
echo "    virtual int on_request(std::map<int, void*>& param);"
echo
echo "    virtual bool encode_body(char* buf, int* size);"
echo "    virtual bool decode_body(const char* buf, int size);"
echo
for i in $FIELD_LIST ; do
echo "    void set_${i}(const char* ptr);"
echo "    std::string get_${i}();"
echo "    void get_${i}(char** ptr, int* size);"
echo
done
echo "public:"
echo "    DECL_LOGGER(logger);"
echo "    static const int BODY_SIZE = ${BODY_SIZE};"
echo "    char m_body[BODY_SIZE];"
echo "};"
echo
echo "#endif"
