#pragma once

#include "System.h"

struct GLFWwindow;

class WindowSystem : public System
{
public:
    WindowSystem();
    
    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void EndFrame();

    bool WindowShouldClose();
    GLFWwindow *GetWindowHandle();

private:
    GLFWwindow *_window;
};
