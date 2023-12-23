#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <map>
#include <string>

class ClientList {
public:
    void AddClient(const std::string& name, bool isConnected);
    void Render();

private:
    std::map<std::string, bool> clients;
};

#endif // CLIENTLIST_H