//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//struct Room {
//    int number;
//    int place;
//    bool conditioner;
//    bool tv;
//    bool reserved;
//    Room(int number, int place, bool conditioner, bool tv) {
//        this->number = number;
//        this->place = place;
//        this->conditioner = conditioner;
//        this->tv = tv;
//        reserved = false;
//    }
//};
//struct Hotel {
//    vector<Room> rooms;
//    Hotel(vector<Room> rooms) {
//        this->rooms = rooms;
//    }
//    void getFreeRooms() {
//        for (const auto& room : rooms) {
//            if (!room.reserved)
//                cout << room.number << " ";
//        }
//    }
//    void getReservedRooms() {
//        for (const auto& room : rooms) {
//            if (room.reserved)
//                cout << room.number << " ";
//        }
//    }
//    void reservedRoom() {
//        int roomNumber; // Номер комнаты которую хотим забронировать
//        cout << "\nВведите номер комнаты:";
//        cin >> roomNumber;
//        bool error = true;
//        for (auto& room : rooms) {
//            if (room.number == roomNumber && !room.reserved) {
//                cout << "Комната успешно забронированна";
//                room.reserved = true;
//                error = false;
//            }
//        }
//        if (error) {
//            cout << "Ошибка при бронировании комнаты";
//        }
//    }
//
//    void filter() {
//        int place;
//        string tv, conditioner;
//        cout << "Кол-во спальных мест: ";
//        cin >> place;
//        cin.ignore();
//        cout << "Наличие TV (Yes/No/Or): ";
//        getline(cin, tv);
//        cout << "Наличие кондиционера (Yes/No/Or): ";
//        getline(cin, conditioner);
//        for (const auto& room : rooms) {
//            if (room.place == place &&
//                ((tv == "Yes" && room.tv) || (tv == "No" && !room.tv) || tv == "Or") &&
//                ((conditioner == "Yes" && room.conditioner) || (conditioner == "No" && !room.conditioner) || conditioner == "Or")) {
//                cout << room.number << " ";
//            }
//        }
//    }
//};
//
//
//
//int main()
//{
//    setlocale(LC_ALL, "RU");
//    Hotel hotel({
//        Room(11, 2, true, true),
//        Room(12, 1, true, false),
//        Room(13, 2, true, true),
//        Room(21, 4, false, false),
//        Room(22, 3, false, true),
//        Room(23, 3, true, false),
//        Room(31, 1, false, true),
//        Room(32, 2, false, true),
//        Room(33, 1, true, false),
//    });
//    string command;
//    cout << "Введите команду:";
//    cin >> command;
//    while (command != "exit") {
//        if (command == "getFreeRooms") {
//            hotel.getFreeRooms();
//        } else if (command == "reserve") {
//            hotel.reservedRoom();
//        } else if (command == "getReservedRooms") {
//            hotel.getReservedRooms();
//        } else if (command == "filter") {
//            hotel.filter();
//        } else {
//            cout << "\nНеверная команда";
//        }
//        cout << "\nВведите команду:";
//        cin >> command;
//    }
//}