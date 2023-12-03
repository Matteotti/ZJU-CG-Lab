#include "ComponentManager.h"

void ComponentManager::EntityDestroyed(Entity entity)
{
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto const &pair : m_componentArrays)
    {
        auto const &componentArray = pair.second;

        componentArray->EntityDestroyed(entity);
    }
}
