#ifdef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;
typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

class sendUDP
{

private:
	SOCKET sock;
public:
	sendUDP();
	bool connexion();
	bool sendMsg(char * buffer, int length);
	bool recupMsg(char * buffer, int length);
	bool closeSocket();
};
#endif
