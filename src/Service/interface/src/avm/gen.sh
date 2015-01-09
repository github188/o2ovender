#!/bin/sh

########## sign in
export HEAD_MACRO="SD_AVM_TEST_REQUEST_H"
export CLASS_NAME="SDAVMTestRequest"
export FIELD_LIST="driver_version soft_version board_version avm_number machine_type drink_number food_number noodle_number drink_set food_set noodle_set"
export LEN_LIST="14 14 4 8 2 1 1 1 2 2 2"
export BODY_SIZE=0
for i in $LEN_LIST
do
    BODY_SIZE=`echo $BODY_SIZE + $i | bc`
done

./h.sh   > ${CLASS_NAME}.h
./cpp.sh > ${CLASS_NAME}.cpp
