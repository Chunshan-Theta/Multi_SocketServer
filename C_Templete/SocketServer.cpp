//server
#include "SocketServer.h"
#include <string>
#include <windows.h>


using namespace std;

SOCKET slisten;
int port;
string HelloSay;
boolean LoopListenOn;
//SOCKET sClient;
//sockaddr_in remoteAddr;

void SocketServer::WinWSAUp() {
	//initialization WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		printf("WSAStartup error !");
	}
}
void SocketServer::TCPServer_initialization(int port) {
	//creat slisten
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		//return 0;
	}
	
	//lock IP and port 
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	//sin.sin_port = htons(27015);
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	//Start listen
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		//return 0;
	}
}

void SocketServer::SendRespond(SOCKET sClient) {
	//hello¡ATCP client
	//char sendData = helloSay.c_str();
	send(sClient, HelloSay.c_str(), strlen(HelloSay.c_str()), 0);
	//closesocket(sClient);
}




void SocketServer::LoopListen() {
	//loop	
	while (1) {
		sockaddr_in remoteAddr;
		int nAddrlen = sizeof(remoteAddr);
		SOCKET sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		

		if (sClient == INVALID_SOCKET)
		{
			printf("Stop Accept!");
		}
		printf("NewListenThread!");

		SocketData socketData;
		socketData.obj = this;
		socketData.socket = sClient;
		socketData.Addr = remoteAddr;

		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SocketServer::threadEntry, &socketData, 0, NULL);
		printf("Create New Thread...\n");
		printf("\n====================================\n");
		
	}
}

void SocketServer::threadEntry(SocketData *so)
{
	
	so->obj->NewListenThread(so->socket,so->Addr);
	
	//so->NewListenThread(&so->sClient, so->remoteAddr);
}


void SocketServer::NewListenThread(SOCKET ThisClient, sockaddr_in ThisRemoteAddr) {
	cout << "NewListenThread" << endl;
	char revData[255];
	int ret=0;
	//Get Data
	while (1) {
		//cout << "NewListenThreadLoop" << endl;
		ret = recv(ThisClient, revData, 255, 0);
		if (ret > 0)
		{
			printf("\nGet a requery¡G%d \r\n", ThisClient);
			revData[ret] = 0x00;
			printf(revData);
		}
		//system("Pause");
		SendRespond(ThisClient);
	}


	
}
void SocketServer::Listen() {
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];

	printf("====================================\n");
	printf("wait...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
	}
	printf("Get a requery¡G%s \r\n", inet_ntoa(remoteAddr.sin_addr));

	//Get Data
	int ret = recv(sClient, revData, 255, 0);
	if (ret > 0)
	{
		revData[ret] = 0x00;
		printf(revData);
	}

	SendRespond(sClient);

}
void SocketServer::StartListen() {
	WinWSAUp();
	TCPServer_initialization(port);


	cout << "Listen();" << endl;
	Listen();


	closesocket(slisten);
	WSACleanup();
	system("Pause");
	//return 0;
};
void SocketServer::KillLoopListen() {
	LoopListenOn = 0;
	closesocket(slisten);
	WSACleanup();
	//return 0;
};
void SocketServer::StartLoopListen() {
	LoopListenOn = 1;
	WinWSAUp();
	TCPServer_initialization(port);
	cout << "LoopListen();" << endl;
	LoopListen();
	KillLoopListen();


};
SocketServer::SocketServer()
{

	HelloSay = "HI Client";
	port = 27015;
	LoopListenOn = TRUE;

}
SocketServer::~SocketServer() {

}