#pragma once

#include "Engine.h"

#include "Entity.h"
#include "Module/EditorModule.h"

#include <memory>
#include <vector>

class RenderSystem;
class WindowSystem;

class Editor
{
public:
    void Init();
    void InitTheme();
    void Run();

    void DockSpace();

private:
    Engine _engine;
    std::shared_ptr<RenderSystem> _renderSystem;
    std::shared_ptr<WindowSystem> _windowSystem;

    std::vector<std::shared_ptr<EditorModule>> _modules;
    bool _reDockFlag = true;
    int _fbWidth, _fbHeight;

    std::shared_ptr<std::vector<Entity>> _entities;
};
