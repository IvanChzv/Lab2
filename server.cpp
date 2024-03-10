#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <chrono>
#define BUFFER_SIZE 1024
std::vector<std::thread> threadPool;
int serverSocket;
void handleClient(int clientSocket, const std::string& saveDirectory) {
    // Generate a unique filename using timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string filename = std::to_string(timestamp);
    // Form the path for saving the file
    std::string savePath = saveDirectory + "/" + filename;
    // Receive the file content from the client
    char buffer[BUFFER_SIZE];
    std::ofstream file(savePath, std::ios::binary);
    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            break;
        }
        file.write(buffer, bytesRead);
    }
    // Close the socket and file
    close(clientSocket);
    file.close();
    std::cout << "File saved: " << savePath << std::endl; // Debug message
}
void signalHandler(int signum) {
    // Terminate all threads
    for (auto& thread : threadPool) {
        thread.join();
    }
    // Close the socket
    close(serverSocket);
    std::cout << "Server terminated" << std::endl; // Debug message
    exit(signum);
}
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: server <port> <max threads> <max file size> <save directory>" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);
    int maxThreads = std::stoi(argv[2]);
    int maxFileSize = std::stoi(argv[3]);
    std::string saveDirectory = argv[4];
    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    }
    // Configure the server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Bind failed" << std::endl;
        return 1;
    }
    // Listen for incoming connections
    if (listen(serverSocket, 10) < 0) {
        std::cout << "Listen failed" << std::endl;
        return 1;
    }
    // Set signal handlers for termination
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    std::cout << "Server started" << std::endl; // Debug message
    while (true) {
        // Accept incoming connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cout << "Accept failed" << std::endl;
            return 1;
        }
        // Create a new thread to handle the client
        threadPool.emplace_back(handleClient, clientSocket, saveDirectory);
        // Limit the number of threads
        if (threadPool.size() >= maxThreads) {
            for (auto& thread : threadPool) {
                thread.join();
            }
            threadPool.clear();
        }
    }
    return 0;
}
