#pragma once
#include <memory>

#include "Components/Rigidbody.h"
#include "System.h"

class PhysicsSystem : public System
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;
};

void ApplyForce(std::shared_ptr<Rigidbody> rigidbody);
void ApplyTorque(std::shared_ptr<Rigidbody> rigidbody);

// bool CheckCollision(std::shared_ptr<