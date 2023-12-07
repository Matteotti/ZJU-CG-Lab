#pragma once

#include <functional>

class Engine
{
public:
    void Init();
    void Run();
    void RunEx(const std::function<void()> &func); // 仅在 Editor 中使用
    void Shutdown();

    void Tick(float dt);

    float CalculateDeltaTime();
};
