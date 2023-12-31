#include "WindowSystem.h"

#include "EngineSettings.h"

#include <chrono>

#include <GLFW/glfw3.h>
#include <cstdio>

void WindowSystem::Init(bool editorMode)
{
    Super::Init(editorMode);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ENGINE_OPENGL_CONTEXT_VER_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ENGINE_OPENGL_CONTEXT_VER_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    _window = glfwCreateWindow(ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT, ENGINE_WINDOW_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(0); // 禁用垂直同步

    _isFirstFrame = true;
}

void WindowSystem::Shutdown()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void WindowSystem::Update(float dt)
{
    static char titleBuf[128];
    static float secCnt = 0.0f;

    if (!_editorMode)
    {
        secCnt += dt;
        if (secCnt >= 0.5f)
        {
            secCnt = 0;
            std::sprintf(titleBuf, ENGINE_WINDOW_TITLE " [FPS: %.0f]", 1.0f / dt);
            glfwSetWindowTitle(_window, titleBuf);
        }
    }
}

int WindowSystem::GetPriority() const
{
    return ENGINE_PRIORITY_WINDOW_SYSTEM;
}

float WindowSystem::CountDeltaTime()
{
    using tp = std::chrono::time_point<std::chrono::steady_clock>;
    static tp prev;

    tp now;
    now = std::chrono::steady_clock::now();
    _deltaTime = 0.9f * _deltaTime +
                 0.1f * std::chrono::duration_cast<std::chrono::microseconds>(now - prev).count() / 1000000.0f;
    prev = now;

    if (_isFirstFrame)
    {
        _isFirstFrame = false;
        _deltaTime = 0.0f;
    }

    return _deltaTime;
}

float WindowSystem::GetDeltaTime() const
{
    return _deltaTime;
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
