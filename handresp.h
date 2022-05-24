#pragma once
#include "stdafx.h"
#include "session.h"
#include "readtext.h"

#define BUFF_SIZE 2043

using namespace std;

/*------------------ Func HandleLogin ---------------------
* To packaging response that send to Client's Login request
* [INPUT] request from Client
* [OUTPUT] 0: Successfully!
*		   11: Account Locked!
*		   12: Not found account!
*		   13: Account is logged in another client!
*/	
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

/*------------------ Func HandlePost ---------------------
* To packaging response that send to Client's Post request
* [INPUT] username: account that logged in client address!
*		  request: from Client!
* [OUTPUT] 20: Successfully!
*		   21: Must log in first!
*/
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

/*------------------ Func HandleLogout ---------------------
* To packaging response that send to Client's Logout request
* [INPUT] username: account that logged in client address!
* [OUTPUT] 30: Successfully!
*		   21: Must log in first!
*/
int handleLogout(string username) {
	if (checkSession(username) == 2 && username.length() > 0) {
		updateStatus("0.0.0.0", 0, username);
		return 30;
	}
	else {
		return 21;
	}
}

/*----------------------- Func getService -----------------------------
* To handle request from client and send response!
* [INPUT] string clientAddr: client address send request!
*		  username: account that logged in client address!
* [OUTPUT] (string) handle service what client want and send message!
*/
string getService(string clientAddr, string request) {
	int reqLen = request.length();
	char tmp[7] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	
	while ((char)request[i] - ' ' != 0 && i < reqLen) {
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
		string username = getPresentSessionUsername(clientAddr);
		if (username.length() == 1) {
			return "21 You must log in to log out =))) !";
		}
		int bye = handleLogout(username);
		switch (bye)
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