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
    std::unordered_map<const char *, Signature> m_signatures{};
    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> m_systems{};
};