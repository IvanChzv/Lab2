#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h> // Добавить эту строку
#include <arpa/inet.h>
#define BUFFER_SIZE 1024
int main(int argc, char* argv[]) {
    if (argc != 4) { // Изменить количество аргументов командной строки на 4
        std::cout << "Usage: client <directory> <filename> <port>" << std::endl; // Изменить сообщение об использовании
        return 1;
    }
    std::string directory = argv[1]; // Добавить аргумент директории
    std::string filename = argv[2];
    int port = std::stoi(argv[3]);
    // Формирование полного пути к файлу
    std::string filePath = directory + "/" + filename;
    // Открытие файла для чтения
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    }
    // Настройка адреса сервера
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    // Установка соединения с сервером
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return 1;
    }
    // Отправка содержимого файла на сервер
    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        send(clientSocket, buffer, file.gcount(), 0);
    }
    // Закрытие сокета и файла
    ::close(clientSocket);
    file.close();
    std::cout << "File sent successfully" << std::endl;
    return 0;
}

