#ifdef _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "sendUDP.h"
#include <winsock2.h>

using  namespace std;

sendUDP::sendUDP()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

sendUDP::connexion()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	struct hostent *hostinfo = NULL;
	SOCKADDR_IN sin = { 0 };
	const char *hostname = "192.168.64.100";

	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL)
	{
		fprintf(stderr, "Unknwo host %s.\n", hostname);
		exit(EXIT_FAILURE);
	}

	sin.sin_addr = *[IN_ADDR *) hostinfo->h_addr;
	sin.sin_port = hstons(1234);
	sin.sin_family = AF_INET;

	if (connect(sock, (SOCKADDR *)&sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		return false;
	}
	return true;

}

sendUDP::sendMsg(char * buffer, int length)
{
	bool resultSend;

	resultSend = send(sock, buffer, length, 0);

	if (resultSend == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

sendUDP::closeSocket()
{
	closesocket(sock);
}

#endif
