#include "CollisionSystem.h"

#include "Components/Collider/BoxCollider.h"
#include "Components/Collider/SphereCollider.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"

#include "Coordinator.h"

#include "EngineSettings.h"

void CollisionSystem::Init(bool editorMode)
{
    Super::Init(editorMode);

    gCoordinator.AddSystemSignature<CollisionSystem, Rigidbody>();
    gCoordinator.AddSystemSignature<CollisionSystem, BoxCollider>();
    gCoordinator.AddSystemSignature<CollisionSystem, Transform>();
}

void CollisionSystem::Shutdown()
{
}

void CollisionSystem::Update(float dt)
{
    // 1. Update Collider & relative pos according to rotation & scale
    //
    // 2. Check for all collisions (considering layer)
    //
    // 3. Resolve all collisions (Apply forces to rigidbodies, considering PhysicsMaterial)
}

int CollisionSystem::GetPriority() const
{
    return ENGINE_PRIORITY_COLLISION_SYSTEM;
}