if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD")
endif()

cmake_minimum_required(VERSION 3.10)
project(Altweb)
set(CMAKE_CXX_STANDARD 14)

# Add ImGui source files
set(IMGUI_DIR "C:/dev/imgui")
set(IMGUI_SOURCES ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_demo.cpp)

# Add ImGui backend source files
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)
set(IMGUI_BACKENDS_SOURCES ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp)

add_executable(Server src/main.cpp src/server.cpp src/ImGuiRenderer.cpp src/ClientList.cpp ${IMGUI_SOURCES} ${IMGUI_BACKENDS_SOURCES})
add_executable(Client src/client/main.cpp ${IMGUI_SOURCES} ${IMGUI_BACKENDS_SOURCES})

# Include ImGui, ImGui backend, and GLFW directories
set(GLFW_DIR "C:/dev/glfw/include")
include_directories(${IMGUI_DIR} ${IMGUI_BACKENDS_DIR} ${GLFW_DIR})

# Link GLFW library
set(GLFW_LIB_DIR "C:/dev/glfw/lib-vc2022")
link_directories(${GLFW_LIB_DIR})
target_link_libraries(Server ws2_32 "${GLFW_LIB_DIR}/glfw3.lib" opengl32)
target_link_libraries(Client ws2_32 "${GLFW_LIB_DIR}/glfw3.lib" opengl32)