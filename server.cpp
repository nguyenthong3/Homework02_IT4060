#include "stdafx.h"
#include "ws2tcpip.h"
#include "winsock2.h"
#include "readtext.h"
#include "handresp.h"
#include "session.h"
#include "thread.h"
#include "handreq.h"
#include <process.h>

#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define MAX_CLIENT 3

#pragma comment (lib,"Ws2_32.lib")
#pragma warning (disable: 4996)

CRITICAL_SECTION criticalSection;
HANDLE myHandle;

/*------ Handle A Sub Thread -------------
*/
unsigned __stdcall echoThread(void* param) {

	cout << "Started Thread!" << endl;
	
	char buff[BUFF_SIZE];
	int ret;

	threadinfo *infor = (threadinfo*)param;

	//WaitForSingleObject(myHandle, INFINITE);
	while (1) {
		EnterCriticalSection(&criticalSection);
		//receive message from client
		ret = recv(infor->sock, buff, BUFF_SIZE, 0);
		//WaitForSingleObject(myHandle, INFINITE);
		if (ret == SOCKET_ERROR) {
			printf("Error %d: Cannot receive data\n", WSAGetLastError());
			LeaveCriticalSection(&criticalSection);
			//SetEvent(myHandle);
			break;
		}
		else if (ret == 0) {
			printf("Client disconnects\n");
			// set session here
			string fullAddr = infor->clientInfor;
			string res = getService(fullAddr, "BYE");
			LeaveCriticalSection(&criticalSection);
			break;
		}
		else {
			buff[ret] = 0;
			string str(buff);
			string fullAddr = infor->clientInfor;
			string res = getService(fullAddr, str);
			int n = res.length();
			buff[n] = '\0';
			strcpy(buff, res.c_str());
			
			//Echo to client
			ret = send(infor->sock,buff, strlen(buff), 0);
			if (ret == SOCKET_ERROR) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					printf("Error %d: Cannot send data\n", WSAGetLastError());
					closesocket(infor->sock);
				}
			}
			//SetEvent(myHandle);
			LeaveCriticalSection(&criticalSection);
		}
	}

	shutdown(infor->sock, SD_SEND);
	closesocket(infor->sock);
	return 0;
}

int main(int argc, char* argv[])
{
	//Check input cmd [Filename.exe PortNumber]
	if (argc != 2 || atoi(argv[1]) < 49152 || atoi(argv[1]) > 65535) {
		cout << "Error input (notice: Port must in [49152;65535])" << endl;
		return 0;
	}

	//step 1: Init Winsock
	WSADATA wsaDATA;
	WORD wVersion = MAKEWORD(2, 2);
	int wsSuccess = WSAStartup(wVersion, &wsaDATA);
	if (wsSuccess == SOCKET_ERROR) {
		cout << "Winsock 2.2 is not support" << endl;
		return 0;
	}

	//step 2: Create non-blocking TCP Server Socket
	SOCKET listenSock;
	unsigned long ul = 1;
	int nRet;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET) {
		printf("Error %d: Cannot create server socket.", WSAGetLastError());
		return 0;
	}

	//Step 3: Bind the socket to an ip address and port
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Can't bind socket!" << endl;
		cout << "Error: " << WSAGetLastError() << endl;
		return 0;
	}

	//Step 04: Listen request from client
	if (listen(listenSock, 10) == SOCKET_ERROR) {
		cout << "Cannot place server socket in state LISTEN: Error " << WSAGetLastError() << endl;
		return 0;
	}

	cout << "Server started!" << endl;

	cout << "----- List user and status ------" << endl;
	listUser();
	cout << "---------------------------------" << endl;

	HANDLE events[4];
	InitializeCriticalSection(&criticalSection);

	//Step 5: Communicate with client
	sockaddr_in clientAddr;
	char clientIP[INET_ADDRSTRLEN];
	int clientAddrLen = sizeof(clientAddr), clientPort;
	


	while (1) {
		SOCKET connSock;
		//accept request
		connSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
		if (connSock == SOCKET_ERROR) {
			printf("Error %d: Cannot permit incoming connection.\n", WSAGetLastError());
			closesocket(connSock);
			break;
		}
		else {
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			clientPort = ntohs(clientAddr.sin_port);
			printf("Accept incoming connection from %s:%d\n", clientIP, clientPort);
			for (int x = 0; x < 4; x++) {
				threadinfo thread;
				thread.clientInfor = getAddr(clientAddr);
				thread.sock = connSock;
				events[x] = (HANDLE)_beginthreadex(0, 0, echoThread, (void*)&thread, 0, 0); //start thread
			}
			//threadinfo thread;
			//thread.clientInfor = getAddr(clientAddr);
			//thread.sock = connSock;
			//_beginthreadex(0, 0, echoThread, (void*)&thread, 0, 0); //start thread
		}
		WaitForMultipleObjects(4, events, TRUE, INFINITE);
	}

	DeleteCriticalSection(&criticalSection);

	//Step 5: Close socket
	closesocket(listenSock);

	//Step 6: Terminate Winsock
	WSACleanup();
	

	return 0;
}