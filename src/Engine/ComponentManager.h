#pragma once
#include "ComponentArray.h"
#include <memory>
#include <unordered_map>

class ComponentManager
{
public:
    template <typename T>
    void RegisterComponent();
    template <typename T>
    ComponentType GetComponentType();
    template <typename T>
    void AddComponent(Entity entity, T component);
    template <typename T>
    void RemoveComponent(Entity entity);
    template <typename T>
    T &GetComponent(Entity entity);

    void EntityDestroyed(Entity entity);

private:
    // Map from type string pointer to a component type
    std::unordered_map<const char *, ComponentType> m_componentTypes;
    // Map from type string pointer to a component
    std::unordered_map<const char *, std::shared_ptr<IComponentArray>> m_componentArrays;
    // The component type to be assigned to the next registered component - starting at 0
    ComponentType m_nextComponentType{};
	
	// Convenience function to get the statically cast pointer to the ComponentArray of type T.
    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray();
};