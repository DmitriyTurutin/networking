// tcp2client.cpp: определяет точку входа для консольного приложения.
//

#pragma comment(lib,"Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

using namespace std;

SOCKET Connect;

void sendMSG()
{
	string msg;
	while (msg != "exit")
	{
		/*cout << "Enter the message:" << endl;*/
		getline(std::cin, msg);
		//cin >> msg;
		send(Connect, msg.c_str(), 1024, NULL);
	}	
	exit(0);
}

void reciveMSG()
{
	char message[1024];
	
	for (;; Sleep(5))
	{
		memset(message, 0, sizeof(message));
		if(recv(Connect, message, sizeof(message), NULL))
			cout << message << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "russian");
	cout << "Client: " << endl;

	char ip[15];
	u_short port;
	//char message[MAX_PATH];
	char message[1024];

	cout << "IP: ";
	cin >> ip;
	cout << endl;

	cout << "Port: ";
	cin >> port;
	cout << endl;

	WSAData wsa;
	WORD Version = MAKEWORD(2, 1);

	WSAStartup(Version, &wsa);

	if (WSAStartup(Version, &wsa) != 0)
	{

		cout << "Can't load WSA library." << endl;

	}
	else
	{

		cout << "WSA library load complite" << endl;

	}

	Connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (Connect == SOCKET_ERROR)
	{

		cout << "Can't create socket." << endl;

	}
	else
	{

		cout << "Socket create complite." << endl;

	}

	SOCKADDR_IN Client;

	Client.sin_family = AF_INET;
	Client.sin_port = htons(port);
	Client.sin_addr.s_addr = inet_addr(ip);
	/*if (connect(Connect, (SOCKADDR*)&Client, sizeof(Client)))*/
	{
		cout << "Connecting to server..." << endl;
		while (1) // !!!
		{

			if (connect(Connect, (SOCKADDR*)&Client, sizeof(Client)))
			{
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendMSG, (LPVOID)(NULL), NULL, NULL);
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)reciveMSG, (LPVOID)(NULL), NULL, NULL);
				for (;; Sleep(1000));
				break;
			}

		}
	}

	ZeroMemory(message, sizeof(message));

	closesocket(Connect);
	WSACleanup();
	system("pause");
	return 0;

}

