#!/bin/sh

echo "#include \"${CLASS_NAME}.h\""
echo
echo "using namespace std;"
echo
echo "IMPL_LOGGER(${CLASS_NAME}, logger);"
echo
echo "${CLASS_NAME}::${CLASS_NAME}()"
echo "{}"
echo
echo "${CLASS_NAME}::~${CLASS_NAME}()"
echo "{}"
echo
echo "SDAVMRequest* ${CLASS_NAME}::clone()"
echo "{"
echo "    return new ${CLASS_NAME}();"
echo "}"
echo
echo "int ${CLASS_NAME}::on_request(std::map<int, void*>& param)"
echo "{"
echo "    LOG4CPLUS_DEBUG(logger, \"Not Imcompleted\");"
echo "    return -1;"
echo "}"
echo 
echo "bool ${CLASS_NAME}::encode_body(char* buf, int* size)"
echo "{"
echo "    *size = ${BODY_SIZE};"
echo "    return true;"
echo "}"
echo 
echo "bool ${CLASS_NAME}::decode_body(const char* buf, int size)"
echo "{"
echo "    return false;"
echo "}"
echo
offset=0
j=0
array=()
for k in ${LEN_LIST} ; do
array=(${array[@]} $k)
done
for i in $FIELD_LIST ; do
echo "void ${CLASS_NAME}::set_${i}(const char* ptr)"
echo "{"
echo "    memcpy(m_body+${offset},ptr,${array[$j]});"
echo "}"
echo "std::string ${CLASS_NAME}::get_${i}()"
echo "{"
echo "    return std::string(m_body+${offset},${array[$j]});"
echo "}"
echo "void ${CLASS_NAME}::get_${i}(char** ptr, int* size)"
echo "{"
echo "    *ptr = m_body + ${offset};"
echo "    *size = ${array[$j]};"
echo "}"
echo
offset=`echo "$offset + ${array[$j]}" | bc`
j=`echo $j + 1 | bc`
done
