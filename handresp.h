#pragma once
#include "stdafx.h"
#include "log.h"
#include "readtext.h"

using namespace std;

int handleLogin(string request) {
	int reqLen = request.length();
	char tmp[20] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	for (int i = 5; i < reqLen; i++) {
		currentChar[0] = request[i];
		strcat(tmp, currentChar);
	}

	string str(tmp);
	cout << str << endl;

	int check = findUser(str);
	//cout << check << endl;

	return check;
}

string handlePost(char* request) {
	return "post";
}

string handleLogout(char* request) {
	return "log out";
}

string getService(string request) {
	int reqLen = request.length();
	char tmp[20] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	while ((char)request[i] - ' ' != 0) {
		currentChar[0] = request[i];
		strcat(tmp, currentChar);
		i++;
	}

	string str(tmp);
	cout << str << endl;
	
	if (str.compare("USER") == 0) {
		//cout << "login handling . . ." << endl;
		switch (handleLogin(request))
		{
			case 0:
				return "Dang nhap thanh cong";
				break;
			case 11:
				return "Tai khoan da bi khoa";
				break;
			case 12:
				return "Khong tim thay tai khoan";
				break;
			default:
				break;
		}
	}
	if (str.compare("POST") == 0) {

	}
	if (str.compare("BYE") == 0) {

	}
	else return "as";
}