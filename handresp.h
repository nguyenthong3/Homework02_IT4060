#pragma once
#include "stdafx.h"
#include "session.h"
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

	if (checkSession(str) == 1) {
		cout << "result check: " << findUser(str) << endl;
		return findUser(str);
	}

	if (checkSession(str) == 2) {
		cout << "result check: 13" << endl;
		return 13;
	}
	return findUser(str);
}

string handlePost(char* request) {
	return "post";
}

string handleLogout(char* request) {
	return "log out";
}

string getService(string clientAddr, string request) {
	int reqLen = request.length();
	char tmp[20] = "";
	char tmp2[20] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	while ((char)request[i] - ' ' != 0) {
		currentChar[0] = request[i];
		strcat(tmp, currentChar);
		i++;
	}

	for (int j = 5; j < reqLen; j++) {
		currentChar[0] = request[j];
		strcat(tmp2, currentChar);
	}

	string str(tmp);
	string username(tmp2);
	cout << "Service name: " << str << endl;
	cout << "Username requested Service: " << username << endl;
	
	if (str.compare("USER") == 0) {
		//cout << "login handling . . ." << endl;
		switch (handleLogin(request))
		{
			case 0:
				updateStatus(clientAddr, 1, username);
				return "10 Login Succesfully!";
				break;
			case 13:
				return "13 Account is logged in another client!";
				break;
			case 11:
				return "11 Account has been locked!";
				break;
			case 12:
				return "12 Username not found!";
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