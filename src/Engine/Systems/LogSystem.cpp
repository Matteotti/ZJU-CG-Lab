#include "LogSystem.h"

#include "EngineSettings.h"

LogSystem::LogSystem()
{
}

void LogSystem::Update(float dt)
{
}

int LogSystem::GetPriority() const
{
    return ENGINE_PRIORITY_LOG_SYSTEM;
}
