#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define IP_SIZE 15

#pragma comment(lib, "Ws2_32.lib")


typedef struct {
	int key;
	int x, y;
} PacketData;

typedef struct {
	int type;
	PacketData data;
} Packet;

int InitializeClient(char* server_ip);
void CleanUpClient();
void Run(SOCKET *connection_socket);