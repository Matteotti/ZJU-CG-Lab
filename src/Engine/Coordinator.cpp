#include "Coordinator.h"

void Coordinator::Init()
{
    m_componentManager = std::make_unique<ComponentManager>();
    m_systemManager = std::make_unique<SystemManager>();
    m_entityManager = std::make_unique<EntityManager>();
}

Entity Coordinator::CreateEntity()
{
    return m_entityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
    m_entityManager->DestroyEntity(entity);

    m_componentManager->EntityDestroyed(entity);

    m_systemManager->EntityDestroyed(entity);
}

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