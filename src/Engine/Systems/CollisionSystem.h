#pragma once
#include <memory>

#include "Components/Rigidbody.h"
#include "System.h"

class CollisionSystem : public System
{
public:
    virtual void Init(bool editorMode) override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;
};