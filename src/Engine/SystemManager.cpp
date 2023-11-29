#include "SystemManager.h"

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    const char *typeName = typeid(T).name();

    assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    m_systems.insert({typeName, system});
    return system;
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{
    const char *typeName = typeid(T).name();

    assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

    // Set the signature for this system
    m_signatures.insert({typeName, signature});
}

void SystemManager::EntityDestroyed(Entity entity)
{
    // Erase a destroyed entity from all system lists
    // m_entities is a set so no check needed
    for (auto const &pair : m_systems)
    {
        auto const &system = pair.second;

        system->m_entities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
    // Notify each system that an entity's signature changed
    for (auto const &pair : m_systems)
    {
        auto const &type = pair.first;
        auto const &system = pair.second;
        auto const &systemSignature = m_signatures[type];

        // Entity signature matches system signature - insert into set
        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->m_entities.insert(entity);
        }
        // Entity signature does not match system signature - erase from set
        else
        {
            system->m_entities.erase(entity);
        }
    }
}