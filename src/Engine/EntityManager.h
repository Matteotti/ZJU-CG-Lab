#pragma once
#include "Entity.h"
#include <array>
#include <queue>

class EntityManager
{
public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

private:
    // Queue of unused entity IDs
    std::queue<Entity> _availableEntities{};

    // Array of signatures where the index corresponds to the entity ID
    std::array<Signature, MAX_ENTITIES> _signatures{};

    // Total living entities - used to keep limits on how many exist
    uint32_t _livingEntityCount{};
};