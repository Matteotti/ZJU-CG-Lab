#include "Context.h"

#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "Entity.h"
#include "Systems/RenderSystem.h"
#include "Systems/WindowSystem.h"

Context gContext;

void Context::Init()
{
    // Init engine & get system handles
    _engine.Init(true);
    _renderSystem = gCoordinator.GetSystem<RenderSystem>();
    _windowSystem = gCoordinator.GetSystem<WindowSystem>();

    _entities = std::make_shared<std::set<Entity>>();

    // Init Camera
    std::shared_ptr<Camera> caComp = std::make_shared<Camera>();
    caComp->SetAsCurrentSceneCamera();
    auto camEntity = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(camEntity, *caComp);
    Transform tfCamComp;
    tfCamComp.SetPosition({0.0f, 0.0f, 30.0f});
    tfCamComp.SetRotation({10.0f, 0.0f, 0.0f});
    gCoordinator.AddComponent(camEntity, tfCamComp);
}
