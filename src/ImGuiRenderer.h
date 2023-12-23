#pragma once

#include <GLFW/glfw3.h>
#include "Server.h"
#include "ClientList.h"
#include <map>
#include <string>

class ImGuiRenderer {
public:
    ImGuiRenderer(GLFWwindow* window, Server& server);
    ~ImGuiRenderer();
    void Render();
    void UpdateClientList(const std::map<std::string, bool>& clientStatus);

private:
    ClientList clientList;
    GLFWwindow* window;
    Server& server;
};