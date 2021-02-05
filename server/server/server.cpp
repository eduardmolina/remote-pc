#include "server.h"


SOCKET CreateSocket() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	return sock;
}

sockaddr_in CreateAddressConfig(char* server_ip) {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(27015);

	inet_pton(AF_INET, server_ip, &addr.sin_addr.s_addr);

	return addr;
}

int InitializeServer(char* server_ip) {
	WSADATA wsa_data;
	
	std::cout << "[+] Starting WSA" << std::endl;

	if (int wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
		std::cout << "[-] Error initializing WSA" << std::endl;

		return wsa_status;
	}

	std::cout << "[+] WSA started" << std::endl;

	std::cout << "[+] Creating socket" << std::endl;

	SOCKET listen_socket = CreateSocket();
	if (listen_socket == INVALID_SOCKET) {
		std::cout << "[-] Error creating socket" << std::endl;

		return SOCKET_ERROR;
	}

	std::cout << "[+] Socket created" << std::endl;

	sockaddr_in addr = CreateAddressConfig(server_ip);

	std::cout << "[+] Binding socket" << std::endl;

	if (bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "[-] Error binding socket" << std::endl;
		closesocket(listen_socket);

		return SOCKET_ERROR;
	}

	std::cout << "[+] Socket binded" << std::endl;

	std::cout << "[+] Set socket to listening" << std::endl;

	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "[-] Error set socket to listening" << std::endl;
		closesocket(listen_socket);

		return SOCKET_ERROR;
	}

	std::cout << "[+] Socket listening" << std::endl;

	return listen_socket;
}

SOCKET WaitingConnection(SOCKET *listen_socket) {

	SOCKET connection_socket;

	do {
		connection_socket = accept(*listen_socket, NULL, NULL);

		if (connection_socket == INVALID_SOCKET)
			std::cout << "[-] Invalid connection socket" << std::endl;

	} while (connection_socket == INVALID_SOCKET);

	return connection_socket;
}

void CleanUpServer() {
	WSACleanup();
}

bool Send(SOCKET *socket, Packet *packet) {
	return send(*socket, (char *)packet, sizeof(*packet), 0) != SOCKET_ERROR ? true : false;
}

void Run(SOCKET *connection_socket) {

	int x_center = GetSystemMetrics(SM_CXSCREEN) / 2;
	int y_center = GetSystemMetrics(SM_CYSCREEN) / 2;

	bool running = true;
	bool remote_mouse = false;

	while (running) {

		int type = -1;
		PacketData data = { 0 };

		if (GetAsyncKeyState(VK_RSHIFT)) {
			std::cout << "[!] Remote mouse turned " << (remote_mouse ? "on" : "off") << std::endl;
			remote_mouse = !remote_mouse;
			Sleep(200);
		}
		else if (GetAsyncKeyState(VK_F9) && !remote_mouse) {
			std::cout << "[!] Client closed" << std::endl;
			type = 9;
			running = false;
			Sleep(200);
		}
		
		if (remote_mouse) {
			POINT point;
			GetCursorPos(&point);

			if (GetAsyncKeyState(VK_RBUTTON)) {
				data.key = -1;
			}
			else if (GetAsyncKeyState(VK_LBUTTON)) {
				data.key = -2;
			}

			type = 1;
			data.x = point.x;
			data.y = point.y;
		}

		if (type != -1) {

			Packet p;
			p.type = type;
			p.data = data;

			Send(connection_socket, &p);
		}
	}
}