#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#define BUFF_SIZE 2048

using namespace std;

/*------------------- Struct session --------------------
* string clientAddr: Address want to connect with server
* bool isLogin: status of account
* string username: account that request from address
*/
typedef struct {
	string clientAddr;
	bool isLogin;
	string username;
} session;

struct threadinfo {
	SOCKET sock;
	string clientInfor;
};


/* Func getAddr: To convert sockaddr_in to string
* [INPUT] sockaddr_in addr: address of client
* [OUTPUT] string fullAddr: address of client by string
*/
string getAddr(sockaddr_in& addr) {
	char IPAddrStr[22];
	inet_ntop(AF_INET, &addr.sin_addr, IPAddrStr, 22);
	string fullAddr;
	fullAddr.append(IPAddrStr);
	fullAddr.append(":");
	fullAddr.append(to_string(addr.sin_port));
	return fullAddr;
}