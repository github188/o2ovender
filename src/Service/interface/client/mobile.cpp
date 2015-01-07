#include <iostream>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "SDSocketUtility.h"
#include "mobile2service.pb.h"

using namespace std;

int send_and_recv(const string&ip, int port, o2ovender::request& request, o2ovender::response& response)
{
	int fd = SDSocketUtility::connect(ip, port);

    cout << request.DebugString() << endl;
    string data;
    request.SerializeToString(&data);

    const int BUFSIZE = 1024*1024;
	char* buffer = new char[BUFSIZE];
	int size = sprintf(buffer, "POST \r\nContent-Length: %u\r\nConnection: close\r\n\r\n", (unsigned)data.length());
    int send_bytes = ::send(fd, buffer, size, MSG_NOSIGNAL);
    send_bytes = ::send(fd, data.c_str(), data.length(), MSG_NOSIGNAL);

    int offset = 0;
    char* body_ptr;
    uint32_t body_length;
    bool done = false;
    for (;;) {
        int recv_bytes = ::recv(fd, buffer+offset, BUFSIZE-offset, 0);
        if (recv_bytes <= 0) {
            cout << "recv fail " << strerror(errno) << endl;
            break;
        }
        offset += recv_bytes;

        char c = buffer[offset];
        buffer[offset] = '\0';
        body_ptr = strstr(buffer, "\r\n\r\n");
        if (!body_ptr) {
            buffer[offset] = c;
            continue;
        }
        body_ptr += 4;
        uint32_t head_length = body_ptr - buffer;

        char* length_ptr = strstr(buffer, "Content-Length:");
        if (!length_ptr) {
            cout << "No Content-Length:" << endl;
            break;
        }
        buffer[offset] = c;

        body_length = atoi(length_ptr+15);
        //cout << "Content-Length:" << body_length << endl;
        if (head_length + body_length > (uint32_t)offset) {
            continue;
        }
        done = true;
        break;
    }
    if (done) {
        response.ParseFromArray(body_ptr, body_length);
        cout << response.DebugString() << endl;
        return true;
    }

	return false;
}

int login_req(int argc, char* argv[])
{
	string ip = "58.61.39.245";
	int port = 80;

    if (argc < 4) {
        cout << "usge: <login> <uid> <passwd>" << endl;
        return 0;
    }
    string uid = argv[2];
    string passwd = argv[3];

    o2ovender::request request;
    request.set_type(o2ovender::request_TYPE_LOGIN);
    o2ovender::login_req* login_req = request.mutable_login_req();
    login_req->set_uid(uid);
    login_req->set_pass_word(passwd);

    o2ovender::response response;
    send_and_recv(ip, port, request, response);
    
	return 0;
}

int register_req(int argc, char* argv[])
{
	string ip = "58.61.39.245";
	int port = 80;

    if (argc < 5) {
        cout << "usge: <register> <uid> <passwd> <code>" << endl;
        return 0;
    }
    string uid = argv[2];
    string passwd = argv[3];
    string code = argv[4];

    o2ovender::request request;
    request.set_type(o2ovender::request_TYPE_REGISTER);
    o2ovender::register_req* register_req = request.mutable_register_req();
    register_req->set_uid(uid);
    register_req->set_pass_word(passwd);
    register_req->set_identifying_code(code);
    
    o2ovender::response response;
    send_and_recv(ip, port, request, response);
    
	return 0;
}

int code_req(int argc, char* argv[])
{
	string ip = "58.61.39.245";
	int port = 80;

    if (argc < 3) {
        cout << "usge: <code> <uid>" << endl;
        return 0;
    }
    string uid = argv[2];

    o2ovender::request request;
    request.set_type(o2ovender::request_TYPE_GET_IDENTIFYING_CODE);
    o2ovender::get_identifying_code_req* get_identifying_code_req = request.mutable_get_identifying_code_req();
    get_identifying_code_req->set_uid(uid);
    
    o2ovender::response response;
    send_and_recv(ip, port, request, response);
    
	return 0;
}

int list_req(int argc, char* argv[])
{
	string ip = "58.61.39.245";
	int port = 80;

    if (argc < 2) {
        cout << "usge: <login>" << endl;
        return 0;
    }

    o2ovender::request request;
    request.set_type(o2ovender::request_TYPE_COMMODITY_LIST);
    o2ovender::commodity_list_req* commodity_list_req = request.mutable_commodity_list_req();

    o2ovender::response response;
    send_and_recv(ip, port, request, response);
    
	return 0;
}
void usage(int argc, char* argv[])
{
    cout << "usage: " << argv[0] << " <register|login|code|list> <args...>" << endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage(argc, argv);
        return 0;
    }

    string request = argv[1];
    if (request == "register") {
        register_req(argc, argv);
    }
    else if (request == "login") {
        login_req(argc, argv);
    }
    else if (request == "code") {
        code_req(argc, argv);
    }
    else if (request == "list") {
        list_req(argc, argv);
    }
    else {
        usage(argc, argv);
    }
    
    return 0;
}
