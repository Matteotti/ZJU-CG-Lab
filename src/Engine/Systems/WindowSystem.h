#pragma once

#include "System.h"

#include <functional>

struct GLFWwindow;

class WindowSystem : public System
{
public:
    virtual void Init(bool editorMode) override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void InitCallbacks();

    void SetDeltaTimeCountMode(bool mode);
    float CountDeltaTime();
    float GetDeltaTime() const;
    void EndFrame();

    void CloseWindow();
    bool WindowShouldClose();
    GLFWwindow *GetWindowHandle();

    void EnableVsync();
    void MaximizeWindow();
    void SetWindowTitle(const std::string &title);

    // Callback
    using OnCursorPosFunc = std::function<void(double, double)>;
    using OnKeyFunc = std::function<void(int, int, int, int)>;
    using OnMouseButtonFunc = std::function<void(int, int, int)>;
    using OnScrollFunc = std::function<void(double, double)>;
    using OnWindowSizeFunc = std::function<void(int, int)>;

    void RegisterOnKeyFunc(OnKeyFunc func);
    void RegisterOnMouseButtonFunc(OnMouseButtonFunc func);
    void RegisterOnCursorPosFunc(OnCursorPosFunc func);
    void RegisterOnScrollFunc(OnScrollFunc func);
    void RegisterOnWindowSizeFunc(OnWindowSizeFunc func);

    void OnKey(int key, int scanCode, int action, int mods);
    void OnMouseButton(int button, int action, int mods);
    void OnCursorPos(double xPos, double yPos);
    void OnScroll(double xOffset, double yOffset);
    void OnWindowSize(int width, int height);

private:
    GLFWwindow *_window;
    bool _isFirstFrame;
    bool _countDeltaTime;
    float _deltaTime;

    std::vector<OnCursorPosFunc> _onCursorPosVec;
    std::vector<OnKeyFunc> _onKeyVec;
    std::vector<OnMouseButtonFunc> _onMouseButtonVec;
    std::vector<OnScrollFunc> _onScrollVec;
    std::vector<OnWindowSizeFunc> _onWindowSizeVec;
};
