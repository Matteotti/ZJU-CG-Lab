#include "TranslateSystem.h"

#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Rigidbody.h"

#include "EngineSettings.h"

extern Coordinator gCoordinator;

void TranslateSystem::Init()
{
}

void TranslateSystem::Update(float dt)
{
    // Iterate over all entities with a Transform component
    for (auto &entity : _entities)
    {
        // Get the Transform component
        auto &transform = gCoordinator.GetComponent<Transform>(entity);
        auto &rigidbody = gCoordinator.GetComponent<Rigidbody>(entity);

        // Update the position
        transform.Translate(rigidbody.GetVelocity() * dt);

        // Update the rotation
        transform.Rotate(rigidbody.GetAngularVelocity() * dt);
    }
}

int TranslateSystem::GetPriority() const
{
    return ENGINE_PRIORITY_TRANSLATE_SYSTEM;
}
