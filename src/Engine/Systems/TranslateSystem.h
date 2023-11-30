#pragma once

#include "System.h"
#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Rigidbody.h"

class TranslateSystem : public System
{
public:
    void Update(float dt);
};