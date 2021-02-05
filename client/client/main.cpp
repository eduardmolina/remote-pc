#include "client.h"


int main() {

	std::cout << "[!] Server IP: ";
	
	char* server_ip = (char *)calloc(IP_SIZE, sizeof(char));
	std::cin >> server_ip;

	std::cout << "[+] Initializing client" << std::endl;

	SOCKET client_socket = InitializeClient(server_ip);

	if (client_socket != SOCKET_ERROR) {

		std::cout << "[+] Client initialized" << std::endl;

		std::cout << "[+] Running" << std::endl;

		Run(&client_socket);

		std::cout << "[+] Stopped" << std::endl;

		closesocket(client_socket);
	}

	std::cout << "[+] Cleaning up" << std::endl;

	CleanUpClient();

	std::cout << "[+] Exiting" << std::endl;

	return 0;
}