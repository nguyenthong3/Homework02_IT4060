﻿#pragma once
#include "stdafx.h"
#include <string>
#include <fstream>

#pragma warning (disable : 4996)

using namespace std;

/*------------------------------------ 
Struct user define a user in.txt file
username: username to login
status: 0 is active, 1 is no active 
*------------------------------------*/
typedef struct {
	string username;
	int status;
} user;

/*------Func getUsername-------------
* To get Username in account.txt
* INPUT: a line in file accout.txt
* OUTPUT: username (string)
*------------------------------------*/
string getUsername(string str) {
	char res[2][20];
	char tmp[20] = "";
	char currentChar[2];
	currentChar[1] = '\0';
	int i = 0;
	int len = str.length();
	for (int i = 0; i < len; i++) {
		if ((char)str[i] - ' ' != 0 && i < len - 1) {
			currentChar[0] = str[i];
			strcat(tmp, currentChar);
			continue;
		}
		else {
			if (i < len - 1 && (char)str[i] - ' ' == 0) {
				int tmpLen = strlen(tmp);
				res[0][tmpLen - 1] = '\0';
				strcpy(res[0], tmp);
			}
			else break;
		}
	}
	return res[0];
}


/*-------Func getStatus------------------
* to Get Status of a user in account.txt
* INPUT: a string line in account.txt
* OUTPUT: status (int) of this user
*---------------------------------------*/
int getStatus(string str) {
	int len = str.length();
	char x[2];
	x[1] = '\0';
	x[0] = str[len - 1];
	int result = atoi(x);
	return result;
}

/*-------

*/
int findUser(string username) {
	fstream file;
	file.open("account.txt", ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			string findUsername = getUsername(line);
			if (username.compare(findUsername) == 0) {
				int status = getStatus(line);
				if (status == 0) {
					return 0;
				}
				else {
					return 11;
				}
			}
		}
		file.close();
	}
	return 12;
}

void listUser() {
	fstream file;
	file.open("account.txt", ios::in);
	if (file.is_open()) {
		string optionalUsername;
		while (getline(file, optionalUsername)) {
			cout << optionalUsername << endl;
		}
		file.close();
	}
}