#include "SystemManager.h"

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