#pragma once
#include <cassert>
#include <memory>
#include <unordered_map>

#include "System.h"

class SystemManager
{
public:
    template <typename T>
    std::shared_ptr<T> RegisterSystem();

    template <typename T>
    void SetSignature(Signature signature);

    void EntityDestroyed(Entity entity);

    void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
    // Map from system type string pointer to a signature
    std::unordered_map<const char *, Signature> _signatures{};
    
    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> _systems{};
};

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    const char *typeName = typeid(T).name();

    assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    _systems.insert({typeName, system});
    return system;
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{
    const char *typeName = typeid(T).name();

    assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

    // Set the signature for this system
    _signatures.insert({typeName, signature});
}
