#pragma once
#include <memory>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

class Coordinator
{
public:
    void Init();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

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
    void SetSystemSignature(Signature signature);

private:
    std::unique_ptr<ComponentManager> m_componentManager;
    std::unique_ptr<SystemManager> m_systemManager;
    std::unique_ptr<EntityManager> m_entityManager;
};