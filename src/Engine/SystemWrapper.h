#pragma once

#include "System.h"

class SystemWrapper
{
public:
    SystemWrapper() = default;
    explicit SystemWrapper(System *ptr_system);

    void Update(float dt);

    bool operator<(SystemWrapper &rhs);

private:
    System *_ptrSystem;
    int _priority;
};
