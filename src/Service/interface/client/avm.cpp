#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "SDSocketUtility.h"
#include "SDAVMRequest.h"
#include "SDAVMSignInRequest.h"
#include "SDAVMSignInResponse.h"

using namespace std;

int send_and_recv(const string&ip, int port, SDAVMRequest& request, SDAVMRequest& response) {
	int fd = SDSocketUtility::connect(ip, port);
    if (fd == -1) {
        cout << "connect fail" << endl;
        return -1;
    }
    cout << "connect " << ip << ":" << port << " succ" << endl;

    const int BUFSIZE = 1024*1024;
	char* buffer = new char[BUFSIZE];
	
    int size = BUFSIZE;
    request.encode(buffer, &size);
   
    int send_bytes = ::send(fd, buffer, size, MSG_NOSIGNAL);
    if (send_bytes != size) {
        cout << "send fail: " << send_bytes << "/" << size << endl;
        return -1;
    }
    cout << "send " << send_bytes << " bytes succ" << endl;

    int recv_bytes = ::recv(fd, buffer, response.HEAD_SIZE, 0);
    if (recv_bytes != response.HEAD_SIZE) {
        cout << "recv head fail" << endl;
        return -1;
    }
    cout << "recv " << recv_bytes << " bytes succ" << endl;

    uint32_t length = *((uint32_t*)buffer);
    int body_length = (4+length) - response.HEAD_SIZE;
    recv_bytes = ::recv(fd, buffer+response.HEAD_SIZE, body_length, 0);
    if (recv_bytes != body_length) {
        cout << "recv body fail: " << recv_bytes << "/" << body_length << endl;
        return -1;
    }
    cout << "recv " << recv_bytes << " bytes succ" << endl;

	return 0;
}

int sign_req(int argc, char* argv[])
{
	string ip = "58.61.39.245";
	int port = 8000;

    if (argc < 2) {
        cout << "usge: <sign>" << endl;
        return 0;
    }

    SDAVMSignInRequest request;
    request.set_control_code("51");
    SDAVMSignInResponse response;
    send_and_recv(ip, port, request, response);
    
	return 0;
}

void usage(int argc, char* argv[])
{
    cout << "usage: " << argv[0] << " <sign>" << endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage(argc, argv);
        return 0;
    }

    string request = argv[1];
    if (request == "sign") {
        sign_req(argc, argv);
    }
    else {
        usage(argc, argv);
    }
    
    return 0;
}
