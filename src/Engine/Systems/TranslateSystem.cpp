#include "TranslateSystem.h"

#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"

#include "EngineSettings.h"

extern Coordinator gCoordinator;

TranslateSystem::TranslateSystem()
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
