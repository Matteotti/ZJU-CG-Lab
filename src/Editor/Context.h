#pragma once

#include "Engine.h"
#include "Entity.h"

#include <cstdint>
#include <memory>
#include <set>

class RenderSystem;
class WindowSystem;

class Context
{
public:
    void Init();

public:
    // Engine
    Engine _engine;
    std::shared_ptr<RenderSystem> _renderSystem;
    std::shared_ptr<WindowSystem> _windowSystem;

    // Editor
    bool _reDockFlag = true;
    bool _sceneRunning = false;

    std::shared_ptr<std::set<Entity>> _entities;
    Entity _selectedEntity = UNDEFINED_ENTITY;
};

extern Context gContext;
