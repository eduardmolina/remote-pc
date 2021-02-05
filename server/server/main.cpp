#include "server.h"


int main() {

	std::cout << "[!] Server IP: ";

	char* server_ip = (char*)calloc(IP_SIZE, sizeof(char));
	std::cin >> server_ip;

	std::cout << "[+] Initializing server" << std::endl;

	SOCKET listen_socket = InitializeServer(server_ip);

	if (listen_socket != SOCKET_ERROR) {

		std::cout << "[+] Server initialized" << std::endl;

		std::cout << "[+] Waiting for connection" << std::endl;

		SOCKET connection_socket = WaitingConnection(&listen_socket);

		std::cout << "[+] Connected" << std::endl;

		std::cout << "[+] Running" << std::endl;

		Run(&connection_socket);

		std::cout << "[+] Stopped" << std::endl;

		closesocket(listen_socket);
	}

	std::cout << "[+] Cleaning up" << std::endl;

	CleanUpServer();

	std::cout << "[+] Exiting" << std::endl;

	return 0;
}