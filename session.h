#pragma once
#include "stdafx.h"
#include "thread.h"

using namespace std;

void writeLog(string log) {
	fstream f;
	f.open("log.txt", ios::app);
	if (f.is_open()) {
		f << log << "\n";
	}
	f.close();
}

/*------------------ Func getSessionAddr ---------------------
* To get client's logged in address in a session
* [INPUT] line: a line in session.txt file
* [OUTPUT] string: client's logged in address
*/
string getSessionAddr(string line) {
	char tmp[22] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	while (line[i] != ' ') {
		currentChar[0] = line[i];
		strcat(tmp, currentChar);
		i++;
	}
	return tmp;
}

/*------------------ Func getSessionIsLogin ---------------------
* To get client's logged in status
* [INPUT] line: a line in session.txt file
* [OUTPUT] string: client's logged in status
*/
int getSessionIsLogin(string line) {
	int i = 0;
	int res;
	while (true) {
		if (line[i] - ' ' == 0) {
			res = (int)line[i + 1];
			break;
		}
		else i++;
	}
	return res-48;
}

/*------------------ Func getSessionAddr ---------------------
* To get client's logged in username in a session
* [INPUT] line: a line in session.txt file
* [OUTPUT] string: client's logged in username
*/
string getSessionUsername(string line) {
	char tmp[20] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	int countSpace = 0;
	while (i < line.length()) {
		if (line[i] - ' ' == 0) {
			countSpace++;
			i++;
			continue;
		}
		if (countSpace == 2) {
			currentChar[0] = line[i];
			strcat(tmp, currentChar);
		}
		i++;
	}
	return tmp;
}

/*------------------ Func getPresentSessionUsername ---------------------
* To get client's logged in username in a session
* [INPUT] line: a line in session.txt file
* [OUTPUT] string: client's logged in username
*/
string getPresentSessionUsername(string addr) {
	string username;
	fstream s;
	s.open("session.txt", ios::in);
	if (s.is_open()) {
		string line;
		while (getline(s, line)) {
			if (getSessionAddr(line).compare(addr) == 0) {
				username = getSessionUsername(line);
			}
		}
	}
	return username;
}

/*------------------ Func checkSession ---------------------
* To know that client has logged in yet
* [INPUT] username: username to check
* [OUTPUT] 1: if Account is logged in
*		   2: if Account is not logged in
*		   99: if Account is not found
*/
int checkSession(string username) {
	fstream file;
	file.open("session.txt", ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (getSessionUsername(line).compare(username) == 0 && getSessionIsLogin(line) == 0) {
				return 1;
			}
			if (getSessionUsername(line).compare(username) == 0 && getSessionIsLogin(line) == 1) {
				return 2;
			}
		}
		file.close();
	}
	return 99;
}

/*------------------ Func updateStatus ---------------------
* [VOID] To update status of account in session
* [INPUT] string addr: Address where account try to log in
*		  int status: status want to set (0: logged out, 1: logged in
*		  string username: username of Account want to set
*/
void updateStatus(string addr, int status, string username) {
	session userSession[4];
	int i = 0;
	fstream file;
	file.open("session.txt", ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			userSession[i].clientAddr = getSessionAddr(line);
			userSession[i].isLogin = getSessionIsLogin(line);
			userSession[i].username = getSessionUsername(line);
			i++;
		}
	}
	file.close();

	for (int c = 0; c < 4; c++) {
		if (userSession[c].username.compare(username) == 0) {
			userSession[c].clientAddr = addr;
			userSession[c].isLogin = status;
		}
		else continue;
	}

	file.open("session.txt", ios::out);

	if (file.is_open()) {
		int j = 0;
		while (j < 4) {
			file << userSession[j].clientAddr << " " << userSession[j].isLogin << " " << userSession[j].username << "\n";
			j++;
		}
	}

	file.close();
}

