#include "ImGuiRenderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

ImGuiRenderer::ImGuiRenderer(GLFWwindow* window, Server& server)
    : window(window), server(server) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup ImGui Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiRenderer::~ImGuiRenderer() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRenderer::UpdateClientList(const std::map<std::string, bool>& clientStatus) {
    for (const auto& client : clientStatus) {
        clientList.AddClient(client.first, client.second);
    }
}

void ImGuiRenderer::Render() {

    // Desired frame rate
    const std::chrono::milliseconds frameTime(1000 / 60);  // 60 FPS

    // ImGui rendering loop
    while (!glfwWindowShouldClose(window)) {
        auto frameStart = std::chrono::steady_clock::now();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update and display the client list
        const auto& clientStatus = server.getClientStatus();
        UpdateClientList(clientStatus);
        clientList.Render();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frameEnd = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        if (frameDuration < frameTime) {
            std::this_thread::sleep_for(frameTime - frameDuration);
        }
    }
}

