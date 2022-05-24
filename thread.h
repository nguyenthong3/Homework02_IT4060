#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

typedef struct {
	string clientAddr;
	bool isLogin;
	string username;
} session;

string getAddr(sockaddr_in& addr) {
	char IPAddrStr[22];
	inet_ntop(AF_INET, &addr.sin_addr, IPAddrStr, 22);
	string fullAddr;
	fullAddr.append(IPAddrStr);
	fullAddr.append(":");
	fullAddr.append(to_string(addr.sin_port));
	return fullAddr;
}