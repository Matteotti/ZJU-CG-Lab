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

    void EntityDestroyed(Entity entity) override;

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