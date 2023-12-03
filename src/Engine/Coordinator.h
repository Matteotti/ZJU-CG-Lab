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


template <typename T>
void Coordinator::RegisterComponent()
{
    m_componentManager->RegisterComponent<T>();
}

template <typename T>
void Coordinator::AddComponent(Entity entity, T component)
{
    m_componentManager->AddComponent<T>(entity, component);

    auto signature = m_entityManager->GetSignature(entity);
    signature.set(m_componentManager->GetComponentType<T>(), true);
    m_entityManager->SetSignature(entity, signature);

    m_systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
void Coordinator::RemoveComponent(Entity entity)
{
    m_componentManager->RemoveComponent<T>(entity);

    auto signature = m_entityManager->GetSignature(entity);
    signature.set(m_componentManager->GetComponentType<T>(), false);
    m_entityManager->SetSignature(entity, signature);

    m_systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
T &Coordinator::GetComponent(Entity entity)
{
    return m_componentManager->GetComponent<T>(entity);
}

template <typename T>
ComponentType Coordinator::GetComponentType()
{
    return m_componentManager->GetComponentType<T>();
}

template <typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
    return m_systemManager->RegisterSystem<T>();
}

template <typename T>
void Coordinator::SetSystemSignature(Signature signature)
{
    m_systemManager->SetSignature<T>(signature);
}
