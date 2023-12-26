#pragma once

#include "System.h"

struct GLFWwindow;

class WindowSystem : public System
{
public:
    virtual void Init(bool editorMode) override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    float CountDeltaTime();
    float GetDeltaTime() const;
    void EndFrame();

    bool WindowShouldClose();
    GLFWwindow *GetWindowHandle();

private:
    GLFWwindow *_window;
    bool _isFirstFrame;
    float _deltaTime;
};
