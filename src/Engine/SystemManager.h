#pragma once
#include <cassert>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>

#include "System.h"
#include "SystemWrapper.h"

class SystemManager
{
public:
    template <typename T>
    std::shared_ptr<T> RegisterSystem();

    template <typename T>
    std::shared_ptr<T> GetSystem();

    template <typename T>
    void SetSignature(Signature signature);

    void EntityDestroyed(Entity entity);

    void EntitySignatureChanged(Entity entity, Signature entitySignature);

    void UpdateSystems(float dt);

private:
    // Map from system type string pointer to a signature
    std::unordered_map<const char *, Signature> _signatures;

    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> _systems;

    // TODO: 增加必要的注释
    std::map<const char*, SystemWrapper> _updateAuxMap;
};

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    const char *typeName = typeid(T).name();

    assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();

    _systems.insert({typeName, system});
    _updateAuxMap.insert({typeName, SystemWrapper(system.get())});

    return system;
}

template <typename T>
std::shared_ptr<T> SystemManager::GetSystem()
{
    const char *typeName = typeid(T).name();

    assert(_systems.find(typeName) != _systems.end() && "This system has not been created yet!");

    return std::static_pointer_cast<T>(_systems[typeName]);
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{
    const char *typeName = typeid(T).name();

    assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

    // Set the signature for this system
    _signatures.insert({typeName, signature});
}
