#pragma once
#include <cassert>
#include <array>
#include <unordered_map>
#include "Entity.h"
#include "Component.h"

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component);

    void RemoveData(Entity entity);

    T &GetData(Entity entity);

    virtual void EntityDestroyed(Entity entity) override;

private:
    // Component array
    std::array<T, MAX_ENTITIES> m_componentArray;

    // Map from an entity ID to an array index
    std::unordered_map<Entity, size_t> m_entityToIndexMap;

    // Map from an array index to an entity ID
    std::unordered_map<size_t, Entity> m_indexToEntityMap;

    // Total size of valid entries in the array
    size_t m_size;
};


template <typename T>
void ComponentArray<T>::InsertData(Entity entity, T component)
{
    assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

    // Put new entry at end and update the maps
    size_t newIndex = m_size;
    m_entityToIndexMap[entity] = newIndex;
    m_indexToEntityMap[newIndex] = entity;
    m_componentArray[newIndex] = component;
    ++m_size;
}

template <typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
    assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");

    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
    size_t indexOfLastElement = m_size - 1;
    m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
    m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    m_entityToIndexMap.erase(entity);
    m_indexToEntityMap.erase(indexOfLastElement);

    --m_size;
}

template <typename T>
T &ComponentArray<T>::GetData(Entity entity)
{
    assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

    // Return a reference to the entity's component
    return m_componentArray[m_entityToIndexMap[entity]];
}

template <typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
    // Remove the entity's component if it existed
    if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
    {
        RemoveData(entity);
    }
}
