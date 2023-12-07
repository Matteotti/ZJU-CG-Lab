#include "Systems/PhysicsSystem.h"

#include "Components/Collider/BoxCollider.h"
#include "Components/Collider/SphereCollider.h"
#include "Components/Rigidbody.h"

#include "EngineSettings.h"

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Update(float dt)
{
    // What should you do in physics system?
    // 1. Update all rigidbodies' velocity according to the forces in the lists
    // 2. Check for all collisions
    // 3. Resolve all collisions (Apply forces to rigidbodies)
}

int PhysicsSystem::GetPriority() const
{
    return ENGINE_PRIORITY_PHYSICS_SYSTEM;
}