#include "SystemManager.h"

void SystemManager::EntityDestroyed(Entity entity)
{
    // Erase a destroyed entity from all system lists
    // _entities is a set so no check needed
    for (auto const &pair : _systems)
    {
        auto const &system = pair.second;

        system->_entities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
    // Notify each system that an entity's signature changed
    for (auto const &pair : _systems)
    {
        auto const &type = pair.first;
        auto const &system = pair.second;
        auto const &systemSignature = _signatures[type];

        // Entity signature matches system signature - insert into set
        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->_entities.insert(entity);
        }
        // Entity signature does not match system signature - erase from set
        else
        {
            system->_entities.erase(entity);
        }
    }
}

void SystemManager::UpdateSystems(float dt)
{
    for (auto &e : _updateAuxMap)
    {
        e.second.Update(dt);
    }
}
