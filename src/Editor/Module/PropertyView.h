#pragma once

#include "EditorModule.h"

#include "Entity.h"

class PropertyView : public EditorModule
{
public:
    PropertyView();
    virtual void Update() override;

private:
    Entity _testEntity; // TEST
};
