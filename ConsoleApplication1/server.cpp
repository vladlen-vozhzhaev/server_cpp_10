#pragma comment(lib, "ws2_32.lib") // ���� ��� ������������ ��� �������������� ����� � ����������� ws2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���������� ��������������
#include <winsock2.h> // ��������� ����� winsock2.h ��� ������ � ��������� Winsock
#include <iostream>

#pragma warrning(disable: 4996)

enum Packet { // ����������� ������������ Packet ��� ����� �������
	P_ChatMessage, // ��� ������ ��� ��������� ����
	P_Test // ��� ��� �������� ��������� ���������
};

int main() {
	WSAData wsaData; // ������ Windows Socket API
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) { // �������������� Winsock
		std::cout << "WSA ERROR" << std::endl;
	}
	else {
		std::cout << "Server started!" << std::endl;
	}

	SOCKADDR_IN addr; // �����
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP_����� (127.0.0.1) - ���������
	addr.sin_port = htons(9123); // ����
	addr.sin_family = AF_INET; // ��������� ����, � ������ ������ IPv4

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // �������� ������ ��� ������������� �������� ����������
	bind(sListen, (SOCKADDR*)&addr, sizeofaddr); // �������� ������ � ������
	listen(sListen, SOMAXCONN); // ������� ������ � ����� ������������� �������� ����������

	SOCKET newConnection; // ����� ��� ������������ ���������� � ����� ��������
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // ��������� �������� ����������
	if (newConnection == 0) {
		std::cout << "ACCEPT ERROR" << std::endl;
	} else {
		std::cout << "Client connected!";
		std::string msg = "Hello world!";
		int msg_size = msg.size();
		Packet msgtype = P_ChatMessage;
		//send(newConnection, (char*)&msgtype, sizeof(Packet), NULL);
		send(newConnection, (char*)&msg_size, sizeof(int), NULL);
		send(newConnection, msg.c_str(), msg_size, NULL);
		while (true){
			int msg_size;
			recv(newConnection, (char*)&msg_size, sizeof(int), NULL);
			char* userMessage = new char[msg_size + 1];
			recv(newConnection, userMessage, msg_size, NULL);
			std::cout << userMessage << std::endl;
		}
		
	}
}