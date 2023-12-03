#include "SphereCollider.h"

SphereCollider::SphereCollider()
{
    _radius = 1.0f;
    _center = glm::vec3(0.0f);
}

void SphereCollider::SetRadius(float radius)
{
    _radius = radius;
}

void SphereCollider::SetCenter(glm::vec3 center)
{
    _center = center;
}

float SphereCollider::GetRadius()
{
    return _radius;
}

glm::vec3 SphereCollider::GetCenter()
{
    return _center;
}