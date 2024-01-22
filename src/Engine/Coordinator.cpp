#include "Coordinator.h"
#include "Entity.h"

void Coordinator::Init()
{
    _componentManager = std::make_unique<ComponentManager>();
    _systemManager = std::make_unique<SystemManager>();
    _entityManager = std::make_unique<EntityManager>();
}

Entity Coordinator::CreateEntity()
{
    return _entityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
    _entityManager->DestroyEntity(entity);

    _componentManager->EntityDestroyed(entity);

    _systemManager->EntityDestroyed(entity);
}

void Coordinator::UpdateSystems(float dt)
{
    _systemManager->UpdateSystems(dt);
}

Signature Coordinator::GetEntitySignature(Entity entity)
{
    return _entityManager->GetSignature(entity);
}
