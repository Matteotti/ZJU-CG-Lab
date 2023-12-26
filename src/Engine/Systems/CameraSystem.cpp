#include "CameraSystem.h"

#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "EngineSettings.h"

void CameraSystem::Init()
{
    gCoordinator.AddSystemSignature<CameraSystem, Camera>();
    gCoordinator.AddSystemSignature<CameraSystem, Transform>();
}

void CameraSystem::Shutdown()
{
}

void CameraSystem::Update(float dt)
{
    for (auto entity : _entities)
    {
        auto &camera = gCoordinator.GetComponent<Camera>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        // LOG_INFO("Update camera's position according to position: %f, %f, %f", transform.GetPosition().x,
        //          transform.GetPosition().y, transform.GetPosition().z);
        camera.TranslateTo(transform.GetPosition());

        // LOG_INFO("Update camera's rotation according to rotation: %f, %f, %f", transform.GetRotation().x,
        //          transform.GetRotation().y, transform.GetRotation().z);
        camera.RotateTo(transform.GetRotation());
    }
}

int CameraSystem::GetPriority() const
{
    return ENGINE_PRIORITY_CAMERA_SYSTEM;
}