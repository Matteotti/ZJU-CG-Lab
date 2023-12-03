#include "SystemWrapper.h"

#include <cassert>

SystemWrapper::SystemWrapper(System *ptr_system)
{
    assert(ptr_system);

    _ptrSystem = ptr_system;
    _priority = ptr_system->GetPriority();
}

void SystemWrapper::Update(float dt)
{
    _ptrSystem->Update(dt);
}

bool SystemWrapper::operator<(SystemWrapper &rhs)
{
    return _priority < rhs._priority;
}
