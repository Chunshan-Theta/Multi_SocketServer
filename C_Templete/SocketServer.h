#ifndef SocketServer_H_INCLUDED
#define SocketServer_H_INCLUDED
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <vector>
#include <string>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <iostream>

using namespace std;

class SocketServer
{
public:	
	int port;
	int action;
	string HelloSay;

	SocketServer();
	~SocketServer();
	void StartListen();
	void StartLoopListen();
	void KillLoopListen();
protected:

private:	
	class SocketData
	{
	public:
		SOCKET socket;
		SocketServer *obj;
		sockaddr_in Addr;
	};


	SOCKET slisten;
	//SOCKET sClient;
	//sockaddr_in remoteAddr;
	boolean LoopListenOn;

	void WinWSAUp();
	void TCPServer_initialization(int port);
	void SendRespond(SOCKET sClient);
	void LoopListen();
	void Listen();
	void NewListenThread(SOCKET C, sockaddr_in R);
	//static void threadEntry(SocketServer *so);
	static void threadEntry(SocketData *so);

};





#endif SocketServer_H_INCLUDED