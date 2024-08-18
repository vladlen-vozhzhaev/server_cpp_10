#pragma once
#include <string>
#include <winsock2.h>
struct User{
	std::string name = "";
	SOCKET socket = NULL;
	std::string getName();
};