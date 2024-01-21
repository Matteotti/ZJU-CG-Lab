#pragma once

#include "Module/EditorModule.h"

#include <memory>
#include <vector>

class Editor
{
public:
    void Init();
    void InitTheme();
    void Run();
    void Shutdown();

    void DockSpace();

private:
    std::vector<std::shared_ptr<EditorModule>> _modules;
};
