#pragma once

#include "EditorModule.h"

#include <string>

class Explorer : public EditorModule
{
public:
    virtual void Update() override;

    bool DiagOpenFile(std::string &filePath, const std::string &filter);
    bool DiagSaveFile(std::string &filePath, const std::string &filter);
};
