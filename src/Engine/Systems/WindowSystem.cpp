#include "WindowSystem.h"

#include "EngineSettings.h"

#include <GLFW/glfw3.h>

WindowSystem::WindowSystem()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ENGINE_OPENGL_CONTEXT_VER_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ENGINE_OPENGL_CONTEXT_VER_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    _window = glfwCreateWindow(ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT, ENGINE_WINDOW_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(0); // 禁用垂直同步
}

void WindowSystem::Update(float dt)
{
}

int WindowSystem::GetPriority() const
{
    return ENGINE_PRIORITY_WINDOW_SYSTEM;
}

void WindowSystem::EndFrame()
{
    glfwPollEvents();
    glfwSwapBuffers(_window);
}

bool WindowSystem::WindowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

GLFWwindow *WindowSystem::GetWindowHandle()
{
    return _window;
}
