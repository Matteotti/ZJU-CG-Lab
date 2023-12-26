#pragma once

#include "System.h"

class CameraSystem : public System
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;
};