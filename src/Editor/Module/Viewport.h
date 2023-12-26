#pragma once

#include "EditorModule.h"

class Viewport : public EditorModule
{
public:
    virtual void Update() override;

private:
    int _viewportNum = 1;
};
