#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>


#pragma comment(lib, "ws2_32.lib")

struct ClientInfo {
    std::string name;
    int nameLength;
};

class Client {
public:
    Client(SOCKET socket, const std::string& name)
        : socket(socket), name(name), isConnected(true) {}

    SOCKET getSocket() const { return socket; }
    const std::string& getName() const { return name; }
    bool getIsConnected() const { return isConnected; } 

    void setConnected(bool connected) { isConnected = connected; }

private:
    SOCKET socket;
    std::string name;
    bool isConnected;
};

class Server {
public:
    Server();
    void run();
    void stop();
    void acceptClients();
    void pingClients();

    bool HasClientStatusChanged();

    const std::map<std::string, bool> getClientStatus() const;

private:
    std::map<std::string, bool> lastClientStatus;
    SOCKET listeningSocket;
    std::map<SOCKET, Client> clients;
    bool running;
};

#endif // SERVER_H