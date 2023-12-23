#include "ClientList.h"
#include "imgui.h"

void ClientList::AddClient(const std::string& name, bool isConnected) {
    clients[name] = isConnected;
}

void ClientList::Render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("Client List", NULL, window_flags);
    for (const auto& client : clients) {
        ImGui::Text("Client: %s, Status: %s", client.first.c_str(), client.second ? "Connected" : "Disconnected");
    }
    ImGui::End();
}