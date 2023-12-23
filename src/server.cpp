#include "Server.h"

Server::Server() {
    WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize winsock\n";
            return;
        }

        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(12345); // Set the port number
        serverAddress.sin_addr.S_un.S_addr = INADDR_ANY; // Listen on any IP address

        listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (listeningSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket\n";
            return;
        }

        if (bind(listeningSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Failed to bind socket\n";
            return;
        }

        if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Failed to listen on socket\n";
            return;
        }
}

void Server::run() {
    running = true;
    while (running) {
        std::thread pingThread(&Server::pingClients, this);
        acceptClients();
        pingThread.detach();
        }
    
}

void Server::stop() {
    running = false;
}


void Server::acceptClients() {
    while (true) {
        sockaddr_in clientAddress{};
        int clientSize = sizeof(clientAddress);

        SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&clientAddress, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client\n";
            continue;
        }
        char name[256];
        int bytesReceived = recv(clientSocket, name, sizeof(name) - 1, 0);
        if (bytesReceived > 0) {
            name[bytesReceived] = '\0';  // Null-terminate the received data
            clients.emplace(clientSocket, Client(clientSocket, name));
            std::cout << "Client " << name << " connected\n";
        }

        // Add a small delay to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}



void Server::pingClients() {
    while (true) {
        for (auto& clientPair : clients) {
            Client& client = clientPair.second;

            if (!client.getIsConnected()) {
                continue;
            }

            SOCKET clientSocket = clientPair.first;
            char pingMsg = 'p';
            int bytesSent = send(clientSocket, &pingMsg, 1, 0);

            if (bytesSent <= 0) {
                // Ping failed, set connection status to false
                client.setConnected(false);
                std::cout << "Client " << client.getName() << " lost connection\n";
                continue;
            }

            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(clientSocket, &readSet);

            timeval timeout;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            if (bytesSent <= 0 || select(0, &readSet, NULL, NULL, &timeout) <= 0) {
                // Ping failed or no data received, set connection status to false
                client.setConnected(false);
                std::cout << "Client " << client.getName() << " lost connection\n";
            } else {
                char buffer[256];
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesReceived <= 0) {
                    // No data received, set connection status to false
                    client.setConnected(false);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));  // Ping clients every 5 seconds
    }
}

bool Server::HasClientStatusChanged() {
    const auto& currentStatus = getClientStatus();
    if (currentStatus != lastClientStatus) {
        lastClientStatus = currentStatus;
        for (const auto& client : currentStatus) {
            std::cout << "Client: " << client.first << ", status: " << (client.second ? "Connected" : "Disconnected") << "\n";
        }
        return true;
    }
    return false;
}

const std::map<std::string, bool> Server::getClientStatus() const {
    std::map<std::string, bool> status;
    for (const auto& client : clients) {
        status[client.second.getName()] = client.second.getIsConnected();
    }
    return status;
}