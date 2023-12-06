#pragma once
#include <memory>
#include <fstream>
#include <chrono>
#include <ctime>

#include "System.h"

class LogSystem : public System
{
public:
    LogSystem();

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

private:
    std::ofstream _fileHandle;
};
