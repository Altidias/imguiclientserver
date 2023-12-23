#include <iostream>
#include <chrono>
#include <thread>
#include "Server.h"
#include "ImGuiRenderer.h"

int main() {
    // Setup GLFW and ImGui
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Server", NULL, NULL);
    glfwMakeContextCurrent(window);

    Server server;

    // Run the server on a separate thread
    std::thread serverThread([&server] { server.run(); });

    ImGuiRenderer imguiRenderer(window, server);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        const auto& clientStatus = server.getClientStatus();
        imguiRenderer.UpdateClientList(clientStatus);
        imguiRenderer.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Wait for the server thread to finish
    server.stop();
    serverThread.join();

    return 0;
}