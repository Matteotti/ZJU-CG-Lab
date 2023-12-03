#include "TranslateSystem.h"

#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Rigidbody.h"

extern Coordinator gCoordinator;

void TranslateSystem::Update(float dt)
{
    // Iterate over all entities with a Transform component
    for (auto &entity : m_entities)
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