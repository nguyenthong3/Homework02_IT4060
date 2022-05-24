#pragma once
#include "stdafx.h"
#include "thread.h"
#include "handresp.h"

#define BUFF_SIZE = 2048

using namespace std;

void handleRequest(char* buff, sockaddr_in& addr) {
	string str(buff);
	string fullAddr = getAddr(addr);
	cout << fullAddr << endl;
	string res = getService(fullAddr, str);
	int n = res.length();
	buff[n] = '\0';
	strcpy(buff, res.c_str());
}