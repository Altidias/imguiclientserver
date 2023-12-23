#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock\n";
        return -1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // The port number should be the same as the server's
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) { // Replace with your server's IP address
        std::cerr << "Failed to set server address\n";
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server\n";
        return -1;
    }

    // Send the client's name to the server
    const char* name = "User1";
    send(clientSocket, name, strlen(name), 0);

    // Main loop
char buffer[256];
while (true) {
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Null-terminate the received data

        if (buffer[0] == 'p') {  // If the received data is a ping message
            char pongMsg = 'p';
            send(clientSocket, &pongMsg, 1, 0);  // Send a pong message back to the server
        } else {
            // Handle other types of messages from the server
        }
    } else if (bytesReceived == 0) {
        // Server has closed the connection
        break;
    } else {
        // Error occurred
        break;
    }
}

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}