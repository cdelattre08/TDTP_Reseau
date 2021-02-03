#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

SOCKADDR_IN InfoServer;

// Mise en place d'une serveur UDP qui écoute sur le port 9013.
const std::vector<std::string> explode(const std::string& msg, const char& c)
{
	std::string buff = "";
	std::vector<std::string> v;
	for (int i = 0; i < msg.length(); i++)
	{
		char compare = msg[i];
		if (compare != c) buff += compare;
		else if (compare == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		v.push_back(buff);

	return v;
}
int main()
{
	// On créer le socket
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	fprintf(stderr, "socket() message: %s\n", strerror(errno));

	InfoServer.sin_addr.s_addr = htonl(INADDR_ANY);
	InfoServer.sin_family = AF_INET;
	InfoServer.sin_port = htons(9013);

	printf("Listage du port %d...\n", 9013);
	
	int i = 0;
	int error_message;
	bool etat = false;
	error_message = ::bind(sock, (SOCKADDR*)&InfoServer, sizeof(InfoServer));

	char buffer[1500], tab[1500];
	memset(buffer, '\0', 1500);
	sockaddr_in _from;
	socklen_t fromlen = sizeof(_from);
	
	do
	{
		// Le server attend de recevoir une trame
		error_message = recvfrom(sock, buffer, 1500, 0, reinterpret_cast<sockaddr*>(&_from), &fromlen);
		// Si le serveur reçoit "quit" alors on arrête le serveur
		int x = 0;
		if (buffer[0] != 'q' && buffer[1] != 'u' && buffer[2] != 'i' && buffer[3] != 't')
		{
			// Gestion d'erreur
			if (error_message < 1)
			{
				fprintf(stderr, "socket() message: %s\n", strerror(errno));
			}
			else
			{
				// On affiche l'ip machine de la machine ip source + le port sur laquelle elle est sorti + le message qu'elle a envoyé
				printf("\nIP : %s \n", inet_ntoa(_from.sin_addr));
				printf("Port : %d \n", ntohs(_from.sin_port));
				printf("Message reçu : %s \n\n", buffer);
				
				std::vector<std::string>cyril = explode(buffer, ':');
				for (int i = 0; i < cyril.size(); i++) {
					x += atoi(cyril[i].c_str());
				}
				std::cout << x << std::endl;
				sprintf(tab, "%d", x);

				sendto(sock, tab, strlen(tab), 0, (struct sockaddr*)&_from, fromlen);

				for (int i = 0; i < 1500; i++) {
					buffer[i] = '\0';
					tab[i] = '\0';
				}
			}
		}
		else
		{
			etat = true;
		}
		
	

	} while (etat != true);

	

	printf("fin du programme\n");
	close(sock);
}

