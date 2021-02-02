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
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

// Mise en place d'un serveur TCP qui écoute sur le port 9013

int main()
{
#if defined (WIN32)
	WSADATA WSAData;
	int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
#else
	int erreur = 0;
#endif


	// Création des deux sockets
	SOCKET sock;
	SOCKADDR_IN sin;
	SOCKET csock;
	SOCKADDR_IN csin;
	char buffer[32] = "Bonjour !";
	char bufferClient[50] = { 0 };
	socklen_t recsize = sizeof(csin);
	int sockBind_err;
	int sockSend_err;
	int sockRecv_err;
	int PORT = 9013;

	if (!erreur)
	{
		// Création du socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		// gestion d'erreur du socket
		if (sock != INVALID_SOCKET)
		{
			// On montre à l'utilisateur que le socket est créé
			printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

			sin.sin_addr.s_addr = htonl(INADDR_ANY);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(PORT);
			// On bind le serveur
			sockBind_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));
			// gestion d'erreur du bind
			if (sockBind_err != SOCKET_ERROR)
			{
				printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
				// Le serveur écoute se met en écoute
				listen(sock, 6);
				// Le serveur accepte la communication avec la machine source
				csock = accept(sock, (SOCKADDR*)&csin, &recsize);
				printf("Un client se connecte avec la socket %d de %s sur le port %d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
				// On reçoit le message envoyé par la source
				sockRecv_err = recv(csock, &bufferClient, sizeof(bufferClient), 0);
				// Gestion d'erreur du nouveau socket
				if (sockRecv_err == -1)
				{
					printf("erreur lors de la reception du message client");
				}
				else
				{
					// Le serveur affiche le message reçu
					printf("Chaine reçu : %s\n", bufferClient);
				}
				// Le serveur envoie "Bonjour !" à la source
				sockSend_err = send(csock, buffer, 32, 0);

				if (sockSend_err != SOCKET_ERROR)
				{
					printf("Chaine envoyée : %s\n", buffer);
				}
				else
				{
					printf("Erreur de transmission\n");

					shutdown(csock, 2);
				}
			}
			else
			{
				printf("Pas reussi à bind le server");
			}

			printf("Fermeture de la socket...\n");
			close(sock);
			printf("Fermeture du serveur terminee\n");
		}
		else
		{
			printf("Pas reussi à creer la socket");
		}

#if defined (WIN32)
		WSACleanup();
#endif
	}
}
