#pragma once

#include "Entity.h"

#include <memory>
#include <vector>

class EditorModule
{
public:
    virtual void Update() = 0;
    virtual ~EditorModule(){};
};
