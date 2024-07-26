#pragma comment(lib, "ws2_32.lib") // Этот код используется для автоматичкской связи с библиотекой ws2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Отключение предупреждений
#include <winsock2.h> // Включение файла winsock2.h для работы с функциями Winsock
#include <iostream>

#pragma warrning(disable: 4996)

SOCKET Connections[100]; // Массив для хранения соединений (сокеты)
int counter = 0; // Счётчик активных соединений
enum Packet { // Определение перечисления Packet для типов пакетов
	P_ChatMessage, // Тип пакета для сообщения чата
	P_Test // Тип для отправки тестового сообщения
};

void ClientHandler(int index) { // функция для обработки клиентского запроса
	while (true) {
		int msg_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* userMessage = new char[msg_size + 1];
		recv(Connections[index], userMessage, msg_size, NULL);
		std::cout << userMessage << std::endl;
		for (int i = 0; i < counter; i++) { // Цикл по всем соединениям
			if (i == index) { // Если это текущее подключение
				continue; // Пропускаем
			}
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL); // Отправка размера сообщения
			send(Connections[i], userMessage, msg_size, NULL); // Отправка сообщения
		}
	}
}

int main() {
	WSAData wsaData; // Данные Windows Socket API
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) { // Инициализируем Winsock
		std::cout << "WSA ERROR" << std::endl;
	}
	else {
		std::cout << "Server started!" << std::endl;
	}

	SOCKADDR_IN addr; // Адрес
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP_адрес (127.0.0.1) - локальный
	addr.sin_port = htons(9123); // Порт
	addr.sin_family = AF_INET; // Семейство сети, в данном случае IPv4

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // Создание сокета для прослушивания входящих соединения
	bind(sListen, (SOCKADDR*)&addr, sizeofaddr); // Привязка адреса к сокету
	listen(sListen, SOMAXCONN); // Перевод сокета в режим прослушивания входящих соединений

	SOCKET newConnection; // Сокет для установления соединения с новым клиентом

	for (int i=0; i<100; i++){
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // Принимаем входящее соединение
		if (newConnection == 0) {
			std::cout << "ACCEPT ERROR" << std::endl;
		}
		else {
			std::cout << "Client connected!" << std::endl;
			std::string msg = "Hello world!\n";
			int msg_size = msg.size();
			Packet msgtype = P_ChatMessage;
			//send(newConnection, (char*)&msgtype, sizeof(Packet), NULL);
			send(newConnection, (char*)&msg_size, sizeof(int), NULL);
			send(newConnection, msg.c_str(), msg_size, NULL);

			Connections[i] = newConnection; // Сохраняем сокет подключившигося клиента в массив соединений
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
}