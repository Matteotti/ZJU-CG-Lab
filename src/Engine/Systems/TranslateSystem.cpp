#include "TranslateSystem.h"

#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "EngineSettings.h"
#include "Systems/LogSystem.h"

void TranslateSystem::Init()
{
    gCoordinator.AddSystemSignature<TranslateSystem, Rigidbody>();
    gCoordinator.AddSystemSignature<TranslateSystem, Transform>();
}

void TranslateSystem::Shutdown()
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
        // LOG_INFO("UPDATE POSITION FROM {%f, %f, %f} TO {%f, %f, %f} ACCORDING TO VELOCITY {%f, %f, %f} AND DT %f",
        //          transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z,
        //          transform.GetPosition().x + rigidbody.GetVelocity().x * dt,
        //          transform.GetPosition().y + rigidbody.GetVelocity().y * dt,
        //          transform.GetPosition().z + rigidbody.GetVelocity().z * dt, rigidbody.GetVelocity().x,
        //          rigidbody.GetVelocity().y, rigidbody.GetVelocity().z, dt);
        transform.Translate(rigidbody.GetVelocity() * dt);

        // Update the rotation
        // LOG_INFO(
        //     "UPDATE ROTATION FROM {%f, %f, %f} TO {%f, %f, %f} ACCORDING TO ANGULAR VELOCITY {%f, %f, %f} AND DT %f",
        //     transform.GetRotation().x, transform.GetRotation().y, transform.GetRotation().z,
        //     transform.GetRotation().x + rigidbody.GetAngularVelocity().x * dt,
        //     transform.GetRotation().y + rigidbody.GetAngularVelocity().y * dt,
        //     transform.GetRotation().z + rigidbody.GetAngularVelocity().z * dt, rigidbody.GetAngularVelocity().x,
        //     rigidbody.GetAngularVelocity().y, rigidbody.GetAngularVelocity().z, dt);
        transform.Rotate(rigidbody.GetAngularVelocity() * dt);
    }
}

int TranslateSystem::GetPriority() const
{
    return ENGINE_PRIORITY_TRANSLATE_SYSTEM;
}
