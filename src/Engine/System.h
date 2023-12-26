#pragma once
#include "Entity.h"
#include <set>

class System
{
public:
    using Super = System;

    virtual void Init(bool editorMode);
    virtual void Shutdown() = 0;

    virtual void Update(float dt) = 0;
    virtual int GetPriority() const = 0;

    virtual ~System()
    {
    }

public:
    std::set<Entity> _entities;

protected:
    bool _editorMode = false;
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