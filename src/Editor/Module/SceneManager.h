#pragma once

#include "EditorModule.h"

class SceneManager : public EditorModule
{
public:
    virtual void Update() override;

    void OnAddEntity();
    void OnClearAllEntity();
};
