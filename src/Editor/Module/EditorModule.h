#pragma once

#include "Entity.h"

#include <memory>
#include <vector>

class EditorModule
{
public:
    virtual void Update() = 0;
    virtual ~EditorModule(){};

public:
    static std::shared_ptr<std::vector<Entity>> _entities;
    static Entity _selectedEntity;
};
