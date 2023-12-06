#pragma once
#include <set>
#include "Entity.h"

class System
{
public:
    virtual void Update(float dt) = 0;
    virtual int GetPriority() const = 0;
    virtual ~System(){}

public:
    std::set<Entity> _entities;
};

// what does system do?
// e.g. PhysicsSystem
// for (auto const &entity : mEntities)
// {
//     // Update the transform component
//     auto &rigidBody = GetComponent<RigidBody>(entity);
//     auto &transform = GetComponent<Transform>(entity);
//     auto const &gravity = GetComponent<Gravity>(entity);

//     transform.position += rigidBody.velocity * dt;
//     rigidBody.velocity += gravity.force * dt;
// }