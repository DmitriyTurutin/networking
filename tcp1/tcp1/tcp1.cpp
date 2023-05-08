// tcp1.cpp: определяет точку входа для консольного приложения.
//

#pragma comment(lib,"Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include "stdafx.h"

//сozd socks
SOCKET Connect;	//priem podkl polzovatel & -> ih v kollect socketov
SOCKET *Connections;  //kollect socketov
SOCKET Listen;

int clientCount=0;

//functsiya rassilki
void sendMSG(int id)
{
	char *buff = new char[1024];
	for (;; Sleep(50))
	{
		memset(buff, 0, sizeof(buff));
		//zapis v buff soobcheniya ot clienta
		if (recv(Connections[id], buff, 1024, NULL))
		{
			printf(buff);
			printf("\n");
			for (int i = 0; i < clientCount; i++)
			{//peredacha soobsh to vsem clientam
				send(Connections[i], buff, strlen(buff), NULL);
			}
			break;
		}
	}
	delete[] buff;
}


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "russian");

	WSAData data;
	WORD version = MAKEWORD(2, 2); //versiya socketov
	//initial socks
	int res = WSAStartup(version, &data);
	if (res != 0)
	{
		//error initial
		return 1;
	}

	//переменные для инициал сок и инф о хосте и пр

	struct addrinfo hints;
	struct addrinfo *result;
	ZeroMemory(&hints, sizeof(hints));



	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));

	//nastroyki socketa
	hints.ai_family = AF_INET; //tip socketa
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//info o hoste
	getaddrinfo(NULL, "1234", &hints, &result);

	//nastroyka listen
	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	//obyavlenie servera
	bind(Listen, result->ai_addr, result->ai_addrlen);

	//obyavlenie servera, kotoriy jdet podkluch
	listen(Listen, SOMAXCONN);
	 
	freeaddrinfo(result);

	printf("server started");

	char m_connect[] = "connected ...  \n";//bufer soobcheniya
	for (;; Sleep(100))
	{
//ojidaem podkluchenie ot clientov
//accept nalichie podcl to server
		if (Connect = accept(Listen, NULL, NULL));
		{
			printf("%d client connected \n", clientCount + 1);
			Connections[clientCount] = Connect;
			send(Connections[clientCount], m_connect, strlen(m_connect), NULL);
			//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendMSG, (LPVOID)(clientCount), NULL, NULL);
			sendMSG(clientCount);
			clientCount++;
			
		}
	}


	return 0;
}

