#pragma once
#include "stdafx.h"
#include "session.h"
#include "readtext.h"

#define BUFF_SIZE 2043

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

int handlePost(string username, string request) {
	if (checkSession(username) == 2) {
		int reqLen = request.length();
		char tmp[BUFF_SIZE] = "";
		char currentChar[2];
		currentChar[1] = '\0';
		for (int i = 5; i < reqLen; i++) {
			currentChar[0] = request[i];
			strcat(tmp, currentChar);
		}
		string article(tmp);
		cout << article << endl;

		fstream file;
		file.open("article.txt", ios::app);
		if (file.is_open()) {
			string line;
			file << getCurrentTime() << " " << username << " " << article << endl;
		}
		file.close();
		return 20;
	}
	else {
		return 21;
	}
}

int handleLogout(string request) {
	return 1;
}

string getService(string clientAddr, string request) {
	int reqLen = request.length();
	char tmp[7] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	while ((char)request[i] - ' ' != 0) {
		currentChar[0] = request[i];
		strcat(tmp, currentChar);
		i++;
	}


	string str(tmp);
	cout << "Service name: " << str << endl;
	
	if (str.compare("USER") == 0) {
		char tmp2[20] = "";
		for (int j = 5; j < reqLen; j++) {
			currentChar[0] = request[j];
			strcat(tmp2, currentChar);
		}
		string username(tmp2);
		cout << "Username requested Service: " << username << endl;
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
				return "99 Not found request";
				break;
		}
	}

	if (str.compare("POST") == 0) {
		string username = getPresentSessionUsername(clientAddr);
		int post = handlePost(username, request);
		switch (post)
		{
			case 20:
				return "20 Post your article successfully!";
				break;
			case 21:
				return "21 You must log in to post article!";
				break;
			default:
				return "99 Not found request";
				break;
		}
	}

	if (str.compare("BYE") == 0) {
		switch (handleLogout(request))
		{
			case 30:
				return "30 Log out success!";
				break;
			case 21:
				return "21 You must log in to Log out =))) !";
				break;
			default:
				return "99 Not found request";
				break;
		}

	}
	else return "99 Not found request";
}