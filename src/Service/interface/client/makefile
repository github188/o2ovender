all : mobile avm

mobile : mobile.cpp ../src/build/common/CMakeFiles/common.dir/SDSocketUtility.cpp.o ../src/build/protocol/CMakeFiles/protocol.dir/mobile2service.pb.cc.o
	g++ -g -Wall -DLOGGER -D_DEBUG $^ -o $@ -I. -I../src/common -I../src/protocol -I/home/root1/log4cplus-1.0.3-rc1/include /home/root1/log4cplus-1.0.3-rc1/lib/liblog4cplus.a -lpthread /usr/local/lib/libprotobuf.a

avm: avm.cpp ../src/build/common/CMakeFiles/common.dir/SDSocketUtility.cpp.o ../src/build/avm/CMakeFiles/avm.dir/SDAVMRequest.cpp.o ../src/build/avm/CMakeFiles/avm.dir/SDAVMSignInRequest.cpp.o ../src/build/common/CMakeFiles/common.dir/SDBuffer.cpp.o ../src/build/avm/CMakeFiles/avm.dir/SDAVMSignInResponse.cpp.o
	g++ -g -Wall -DLOGGER -D_DEBUG $^ -o $@ -I. -I../src -I../src/common -I../src/main -I../src/avm -I/home/root1/log4cplus-1.0.3-rc1/include /home/root1/log4cplus-1.0.3-rc1/lib/liblog4cplus.a -lpthread
