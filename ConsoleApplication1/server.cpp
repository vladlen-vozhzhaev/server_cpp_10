#pragma comment(lib, "ws2_32.lib") // Этот код используется для автоматичкской связи с библиотекой ws2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Отключение предупреждений
#include <winsock2.h> // Включение файла winsock2.h для работы с функциями Winsock
#include <iostream>
#include <cstring>
#include "User.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#pragma warrning(disable: 4996)

User users[100];
int counter = 0; // Счётчик активных соединений

bool disconnectClient(int result, int index) {
	if (result <= 0) {
		closesocket(users[index].socket);
		users[index] = users[counter - 1];
		counter--;
		return true;
	}
	return false;
}

void sendMessage(SOCKET socket, std::string message) {
	int msg_size = message.size();
	send(socket, (char*)&msg_size, sizeof(int), NULL);
	send(socket, message.c_str(), msg_size, NULL);
}
std::string receiveMessage(SOCKET socket, int index) {
	int msg_size;
	int result = recv(socket, (char*)&msg_size, sizeof(int), NULL);
	if (disconnectClient(result, index)) return "client disconected";
	char* msg = new char[msg_size + 1];
	recv(users[index].socket, msg, msg_size, NULL);
	msg[msg_size] = '\0';
	return msg;
}

void ClientHandler(int index) { // функция для обработки клиентского запроса
	sendMessage(users[index].socket, "Welcome!\nPlease select command (/login or /register)");
	std::string command = receiveMessage(users[index].socket, index);
	if (!strcmp(command.c_str(), "/login")) {
		std::cout << "User command /login" << std::endl;
		sendMessage(users[index].socket, "Enter login:");
		std::string login = receiveMessage(users[index].socket, index);

		sendMessage(users[index].socket, "Enter pass:");
		std::string pass = receiveMessage(users[index].socket, index);
		sql::Driver* driver;
		sql::Connection* con;
		sql::ResultSet* res;
		sql::PreparedStatement* pstmt;
		try {
			driver = get_driver_instance();
			con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		}
		catch (sql::SQLException e) {
			std::cout << "connect error\n";
			system("pause");
			exit(1);
		}
		try {
			con->setSchema("cpp10_chat");
			std::cout << login + '\n' + pass;
			pstmt = con->prepareStatement("SELECT * FROM users WHERE login = ? AND pass= ?");
			pstmt->setString(1, login);
			pstmt->setString(2, pass);
			res = pstmt->executeQuery();
			res->next();
			std::cout << "ID:" << res->getInt("id") << ", Name: " << res->getString("name") << std::endl;
		}
		catch (sql::SQLException& e) {
			std::cerr << "SQLException" << e.what() << std::endl;
			std::cerr << "SQLEState" << e.getSQLState() << std::endl;
		}
		
	}
	else if (!strcmp(command.c_str(), "/register")) {
		std::cout << "User command /register" << std::endl;
		sendMessage(users[index].socket, "Enter name:");
		std::string name = receiveMessage(users[index].socket, index);

		sendMessage(users[index].socket, "Enter login:");
		std::string login = receiveMessage(users[index].socket, index);
		
		sendMessage(users[index].socket, "Enter pass:");
		std::string pass = receiveMessage(users[index].socket, index);

		std::cout << name + "\n" + login + "\n" + pass + "\n";
		sql::Driver* driver;
		sql::Connection* con;
		sql::PreparedStatement* pstmt;
		try {
			driver = get_driver_instance();
			con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		}
		catch (sql::SQLException e) {
			std::cout << "connect error\n";
			system("pause");
			exit(1);
		}
		con->setSchema("cpp10_chat");
		pstmt = con->prepareStatement("INSERT INTO users (name, login, pass) VALUES (?,?,?)");
		pstmt->setString(1, name);
		pstmt->setString(2, login);
		pstmt->setString(3, pass);
		pstmt->executeUpdate();
	}
	else {
		std::cout << "User command:" << command << std::endl;
	}
		
	users[index].name = "";//name;

	while (true) {
		int msg_size;
		int result = recv(users[index].socket, (char*)&msg_size, sizeof(int), NULL);

		if (result <= 0) {
			std::cout << "Client" << users[index].name << "disconnected\n";
			for (int i = 0; i < counter; i++) { // Цикл по всем соединениям
				if (i != index) { // Если это текущее подключение
					sendMessage(users[i].socket, "Client disconnected" + users[index].name);
				}
			}
			closesocket(users[index].socket);
			users[index] = users[counter - 1];
			counter--;
			break;
		}

		char* userMessage = new char[msg_size + 1];
		recv(users[index].socket, userMessage, msg_size, NULL);
		userMessage[msg_size] = '\0';
		std::string response = users[index].name + ": " + userMessage;
		std::cout << response << std::endl;
		for (int i = 0; i < counter; i++) { // Цикл по всем соединениям
			if (i == index) { // Если это текущее подключение
				continue; // Пропускаем
			}
			sendMessage(users[i].socket, response);
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
			User user;
			user.socket = newConnection;
			users[i] = user;
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
}