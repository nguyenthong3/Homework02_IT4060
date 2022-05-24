#include "stdafx.h"
#include "ws2tcpip.h"
#include "winsock2.h"
#include "readtext.h"
#include "handresp.h"
#include "session.h"
#include "thread.h"

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define MAX_CLIENT 3

#pragma comment (lib,"Ws2_32.lib")
#pragma warning (disable: 4996)

///* echoThread - Thread to receive the message from client and echo*/
//unsigned __stdcall echoThread(void* param) {
//	char buff[BUFF_SIZE];
//	int ret;
//	sockaddr_in clientAddr;
//	char clientIP[INET_ADDRSTRLEN];
//	int clientAddrLen = sizeof(clientAddr), clientPort;
//
//	SOCKET connectedSocket = (SOCKET)param;
//	ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
//
//	if (ret == SOCKET_ERROR)
//		printf("Error %d: Cannot receive data.\n", WSAGetLastError());
//	else if (ret == 0)
//		printf("Client disconnects.\n");
//	else if (strlen(buff) > 0) {
//
//		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
//		clientPort = ntohs(clientAddr.sin_port);
//		buff[ret] = 0;
//		printf("Receive from client[%s:%d] %s\n", clientIP, clientPort, buff);
//		//Echo to client
//		ret = send(connectedSocket, buff, strlen(buff), 0);
//		if (ret == SOCKET_ERROR)
//			printf("Error %d: Cannot send data.\n", WSAGetLastError());
//	}
//
//	closesocket(connectedSocket);
//	return 0;
//}

int main(int argc, char* argv[])
{
	////Check input cmd [Filename.exe PortNumber]
	//if (argc != 2 || atoi(argv[1]) < 49152 || atoi(argv[1]) > 65535) {
	//	cout << "Error input (notice: Port must in [49152;65535])" << endl;
	//	return 0;
	//}

	//step 1: Init Winsock
	WSADATA wsaDATA;
	WORD wVersion = MAKEWORD(2, 2);
	int wsSuccess = WSAStartup(wVersion, &wsaDATA);
	if (wsSuccess == SOCKET_ERROR) {
		cout << "Winsock 2.2 is not support" << endl;
		return 0;
	}

	//step 2: Create TCP Server Socket
	SOCKET listenSock;
	unsigned long ul = 1;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET) {
		printf("Error %d: Cannot create server socket.", WSAGetLastError());
		return 0;
	}

	if (ioctlsocket(listenSock, FIONBIO, (unsigned long*)&ul)) {
		printf("Error! Cannot change to non-blocking mode.");
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

	//Step 5: Communicate with client
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE];
	int i, ret, clientAddrLen = sizeof(clientAddr);
	SOCKET client[MAX_CLIENT];

	for (i = 0; i < MAX_CLIENT; i++)
		client[i] = 0;

	while (1) {
		SOCKET connSock;

		//accept request
		connSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
			if (connSock != SOCKET_ERROR) {
				for (i = 0; i < MAX_CLIENT; i++)
					if (client[i] == 0) {
						client[i] = connSock;
						break;
					}
				if (i == MAX_CLIENT) {
					printf("Error: Cannot response more client.\n");
					closesocket(connSock);
				}
			}

		for (i = 0; i < MAX_CLIENT; i++) {
			if (client[i] == 0) continue;

			//receive message from client
			ret = recv(client[i], buff, BUFF_SIZE, 0);
			if (ret == SOCKET_ERROR) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					printf("Error %d: Cannot receive data\n", WSAGetLastError());
					closesocket(client[i]);
					client[i] = 0;
				}

			}
			else if (ret == 0) {
				printf("Client disconnects\n");
				closesocket(client[i]);
				client[i] = 0;
			}
			else {
				buff[ret] = 0;
				printf("Receive from client[%s:%d] %s\n",
					inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
				string str(buff);
				string fullAddr = getAddr(clientAddr);
				cout << fullAddr << endl;
				string res = getService(fullAddr,str);
				int n = res.length();
				buff[n] = '\0';
				strcpy(buff, res.c_str());

				//Echo to client
				ret = send(client[i],buff, strlen(buff), 0);
				if (ret == SOCKET_ERROR) {
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						printf("Error %d: Cannot send data\n", WSAGetLastError());
						closesocket(client[i]);
						client[i] = 0;
					}
				}
			}
		}
	} //end accepting

	//Step 5: Close socket
	closesocket(listenSock);

	//Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}