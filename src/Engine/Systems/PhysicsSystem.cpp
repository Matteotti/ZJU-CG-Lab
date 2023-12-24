#include "Systems/PhysicsSystem.h"

#include "Components/Rigidbody.h"
#include "Coordinator.h"

#include "EngineSettings.h"

void PhysicsSystem::Init()
{
    gCoordinator.AddSystemSignature<PhysicsSystem, Rigidbody>();
}

void PhysicsSystem::Shutdown()
{
}

void PhysicsSystem::Update(float dt)
{
    // What should you do in physics system?
    for (auto entity : _entities)
    {
        // 1. Update all rigidbodies' velocity according to the forces in the lists
        // 1.1. Update common forces
        // 1.2. Update relative forces
        // 1.3. Update common torques
        // 1.4. Update relative torques
    }
}

int PhysicsSystem::GetPriority() const
{
    return ENGINE_PRIORITY_PHYSICS_SYSTEM;
}