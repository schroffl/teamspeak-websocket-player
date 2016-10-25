#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>

#ifdef _WIN32
	#include <Windows.h>
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
#endif

int init() {
	#ifdef _WIN32
		WSADATA wsaData;
		if (WSAStartup(0x0202, &wsaData) != 0) {
			printf("WSAStartup failed: %d\n", WSAGetLastError());
			return -1;
		}
	#endif
	
	return 0;
}

void closeSock(int sock) {
	printf("Closing socket %d\n", sock);

	#ifdef _WIN32
		closesocket(sock);
		WSACleanup();
	#else
		close(sock);
	#endif
}

int startListen(int port) {
	if(init() != 0)
		return -1;

	int sock;
	struct sockaddr_in destination;

	destination.sin_family = AF_INET;
	sock = (int) socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock < 0) {
		printf("Socket Creation Failed\n");
		closeSock(sock);
		return -1;
	}

	destination.sin_port = htons(port);
	destination.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr*) &destination, sizeof(destination)) < 0) {
		printf("Failed to bind socket\n");
		closeSock(sock);
		return -1;
	}

	if (listen(sock, 5) < 0) {
		printf("listen() failed\n");
		closeSock(sock);
		return -1;
	}

	printf("Listening on port %d\n", port);

	return sock;

	/*struct sockaddr_in clientAddress;
	int clientSize = sizeof(clientAddress);
	int client = accept(sock, (struct sockaddr*) &clientAddress, &clientSize);

	if (client < 0)
		printf("\naccept() failed");
	else
		printf("\nClient connected");

	char buffer[256];

	while (1) {
		int numbytes = recv(client, buffer, 256, 0);

		if (numbytes > 0)
			printf("\nGot %d bytes", numbytes);
		else if (numbytes < 0)
			break;
	}

	printf("\nClient disconnected");

	closeSock(sock);*/
}