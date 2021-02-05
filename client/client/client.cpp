#include "client.h"


SOCKET CreateSocket() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	return sock;
}

sockaddr_in CreateAddressConfig(char *server_ip) {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(27015);

	inet_pton(AF_INET, server_ip, &addr.sin_addr.s_addr);

	return addr;
}

int InitializeClient(char *server_ip) {
	WSADATA wsa_data;

	std::cout << "[+] Starting WSA" << std::endl;

	if (int wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
		std::cout << "[-] Error initializing WSA" << std::endl;

		return wsa_status;
	}

	std::cout << "[+] WSA started" << std::endl;

	std::cout << "[+] Creating socket" << std::endl;

	SOCKET client_socket = CreateSocket();
	if (client_socket == INVALID_SOCKET) {
		std::cout << "[-] Error creating socket" << std::endl;

		return SOCKET_ERROR;
	}

	std::cout << "[+] Socket created" << std::endl;

	sockaddr_in addr = CreateAddressConfig(server_ip);

	std::cout << "[+] Connecting" << std::endl;

	if (connect(client_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "[-] Error on connecting to " << server_ip << std::endl;
		closesocket(client_socket);

		return SOCKET_ERROR;
	}

	std::cout << "[+] Connected" << std::endl;

	return client_socket;

}

void CleanUpClient() {
	WSACleanup();
}

bool Recv(SOCKET *connection_socket, Packet *packet) {

	return recv(*connection_socket, (char*)packet, sizeof(Packet), 0) != INVALID_SOCKET ? true : false;
}

void Click(int button, int x, int y) {
	mouse_event(button == -1 ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
	mouse_event(button == -2 ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, x, y, 0, 0);
}

void Run(SOCKET *connection_socket) {

	while (true) {

		Packet p;

		if (Recv(connection_socket, &p)) {

			int type = p.type;

			if (type == 9) {
				break;
			}

			if (type == 1) {
				PacketData data = p.data;
				SetCursorPos(data.x, data.y);

				if (data.key)
					Click(data.key, data.x, data.y);
			}
		}
	}
}
