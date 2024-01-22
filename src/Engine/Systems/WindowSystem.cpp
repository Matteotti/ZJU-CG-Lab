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

    InitCallbacks();

    _isFirstFrame = true;
    _countDeltaTime = true;
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

    if (!_countDeltaTime)
        return 0.0f;

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

void WindowSystem::SetDeltaTimeCountMode(bool mode)
{
    if (!mode)
        _isFirstFrame = true;
    _countDeltaTime = mode;
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

void WindowSystem::EnableVsync()
{
    glfwSwapInterval(1);
}

void WindowSystem::MaximizeWindow()
{
    glfwMaximizeWindow(_window);
}

void WindowSystem::SetWindowTitle(const std::string &title)
{
    glfwSetWindowTitle(_window, title.c_str());
}

void WindowSystem::CloseWindow()
{
    glfwSetWindowShouldClose(_window, true);
}

void WindowSystem::InitCallbacks()
{
    glfwSetWindowUserPointer(_window, this);

    glfwSetCursorPosCallback(_window, [](GLFWwindow *window, double xPos, double yPos) {
        ((WindowSystem *)glfwGetWindowUserPointer(window))->OnCursorPos(xPos, yPos);
    });
    glfwSetKeyCallback(_window, [](GLFWwindow *window, int key, int scanCode, int action, int mods) {
        ((WindowSystem *)glfwGetWindowUserPointer(window))->OnKey(key, scanCode, action, mods);
    });
    glfwSetMouseButtonCallback(_window, [](GLFWwindow *window, int button, int action, int mods) {
        ((WindowSystem *)glfwGetWindowUserPointer(window))->OnMouseButton(button, action, mods);
    });
    glfwSetScrollCallback(_window, [](GLFWwindow *window, double xOffset, double yOffset) {
        ((WindowSystem *)glfwGetWindowUserPointer(window))->OnScroll(xOffset, yOffset);
    });
    glfwSetWindowSizeCallback(_window, [](GLFWwindow *window, int width, int height) {
        ((WindowSystem *)glfwGetWindowUserPointer(window))->OnWindowSize(width, height);
    });
}

void WindowSystem::RegisterOnCursorPosFunc(OnCursorPosFunc func)
{
    _onCursorPosVec.push_back(func);
}

void WindowSystem::RegisterOnKeyFunc(OnKeyFunc func)
{
    _onKeyVec.push_back(func);
}

void WindowSystem::RegisterOnMouseButtonFunc(OnMouseButtonFunc func)
{
    _onMouseButtonVec.push_back(func);
}

void WindowSystem::RegisterOnScrollFunc(OnScrollFunc func)
{
    _onScrollVec.push_back(func);
}

void WindowSystem::RegisterOnWindowSizeFunc(OnWindowSizeFunc func)
{
    _onWindowSizeVec.push_back(func);
}

void WindowSystem::OnCursorPos(double xPos, double yPos)
{
    for (auto &fn : _onCursorPosVec)
        fn(xPos, yPos);
}

void WindowSystem::OnKey(int key, int scanCode, int action, int mods)
{
    for (auto &fn : _onKeyVec)
        fn(key, scanCode, action, mods);
}

void WindowSystem::OnMouseButton(int button, int action, int mods)
{
    for (auto &fn : _onMouseButtonVec)
        fn(button, action, mods);
    ;
}

void WindowSystem::OnScroll(double xOffset, double yOffset)
{
    for (auto &fn : _onScrollVec)
        fn(xOffset, yOffset);
}

void WindowSystem::OnWindowSize(int width, int height)
{
    for (auto &fn : _onWindowSizeVec)
        fn(width, height);
}
