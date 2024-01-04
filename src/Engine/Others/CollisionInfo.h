#pragma once
#include "Components/Rigidbody.h"
#include "Others/PhysicsMaterial.h"
#include <memory>

class CollisionInfo
{
public:
    bool _isColliding;

    // Direction: Self to other
    glm::vec3 _collisionNormal;
    glm::vec3 _selfCollisionPoint;
    glm::vec3 _otherCollisionPoint;

    CollisionInfo(bool isColliding, glm::vec3 selfCollisionPoint, glm::vec3 otherCollisionPoint)
        : _isColliding(isColliding), _selfCollisionPoint(selfCollisionPoint), _otherCollisionPoint(otherCollisionPoint)
    {
        _collisionNormal = glm::normalize(otherCollisionPoint - selfCollisionPoint);
    }

    CollisionInfo(bool isColliding) : _isColliding(isColliding)
    {
    }
};