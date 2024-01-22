#pragma once
#include <memory>

#include "ComponentManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"

class Coordinator
{
public:
    void Init();

    [[nodiscard]] Entity CreateEntity();

    void DestroyEntity(Entity entity);

    [[nodiscard]] Signature GetEntitySignature(Entity entity);

    void UpdateSystems(float dt);

    template <typename Tcomp>
    [[nodiscard]] bool HasComponent(Entity entity);

    template <typename T>
    void RegisterComponent();

    template <typename T>
    void AddComponent(Entity entity, T component);

    template <typename T>
    void RemoveComponent(Entity entity);

    template <typename T>
    T &GetComponent(Entity entity);

    template <typename T>
    ComponentType GetComponentType();

    template <typename T>
    std::shared_ptr<T> RegisterSystem();

    template <typename T>
    void DestorySystem();

    template <typename T>
    std::shared_ptr<T> GetSystem();

    template <typename T, typename Tcomp>
    void AddSystemSignature();

    template <typename T>
    [[nodiscard]] Signature GetSystemSignature();

private:
    std::unique_ptr<ComponentManager> _componentManager;
    std::unique_ptr<SystemManager> _systemManager;
    std::unique_ptr<EntityManager> _entityManager;
};

extern Coordinator gCoordinator;

template <typename T>
void Coordinator::RegisterComponent()
{
    _componentManager->RegisterComponent<T>();
}

template <typename T>
void Coordinator::AddComponent(Entity entity, T component)
{
    _componentManager->AddComponent<T>(entity, component);

    auto signature = _entityManager->GetSignature(entity);
    signature.set(_componentManager->GetComponentType<T>(), true);
    _entityManager->SetSignature(entity, signature);

    _systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
void Coordinator::RemoveComponent(Entity entity)
{
    _componentManager->RemoveComponent<T>(entity);

    auto signature = _entityManager->GetSignature(entity);
    signature.set(_componentManager->GetComponentType<T>(), false);
    _entityManager->SetSignature(entity, signature);

    _systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
T &Coordinator::GetComponent(Entity entity)
{
    return _componentManager->GetComponent<T>(entity);
}

template <typename T>
ComponentType Coordinator::GetComponentType()
{
    return _componentManager->GetComponentType<T>();
}

template <typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
    return _systemManager->RegisterSystem<T>();
}

template <typename T>
void Coordinator::DestorySystem()
{
    return _systemManager->DestorySystem<T>();
}

template <typename T>
std::shared_ptr<T> Coordinator::GetSystem()
{
    return _systemManager->GetSystem<T>();
}

template <typename T, typename Tcomp>
void Coordinator::AddSystemSignature()
{
    _systemManager->AddSignature<T>(GetComponentType<Tcomp>());
}

template <typename T>
Signature Coordinator::GetSystemSignature()
{
    return _systemManager->GetSignature<T>();
}

template <typename Tcomp>
[[nodiscard]] bool Coordinator::HasComponent(Entity entity)
{
    auto compType = _componentManager->GetComponentType<Tcomp>();
    return _entityManager->GetSignature(entity).test(compType);
}
