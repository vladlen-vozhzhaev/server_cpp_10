//#include <iostream>
//#include "mysql_connection.h"
//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/prepared_statement.h>
//#include <cppconn/resultset.h>
//
//int main() {
//	sql::Driver* driver;
//	sql::Connection* con;
//	sql::PreparedStatement* pstmt;
//	sql::ResultSet* res;
//	try {
//		driver = get_driver_instance();
//		con = driver->connect("tcp://127.0.0.1:3306", "root", "");
//	}
//	catch (sql::SQLException e) {
//		std::cout << "connect error\n";
//		system("pause");
//		exit(1);
//	}
//	con->setSchema("cpp10_chat");
//	//pstmt = con->prepareStatement("INSERT INTO users (name, login, pass) VALUES ('Petr','petr@mail.ru', '123')");
//	//pstmt->execute();
//	pstmt = con->prepareStatement("SELECT * FROM users");
//	res = pstmt->executeQuery();
//	while (res->next()){
//		std::cout << "ID:" << res->getInt("id") << ", Name: " << res->getString("name") << std::endl;
//	}
//	delete pstmt;
//	delete con;
//
//}