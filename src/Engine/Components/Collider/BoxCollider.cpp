#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
    _size = glm::vec3(1.0f);
    _center = glm::vec3(0.0f);
}

void BoxCollider::SetSize(glm::vec3 size)
{
    _size = size;
}

void BoxCollider::SetCenter(glm::vec3 center)
{
    _center = center;
}

glm::vec3 BoxCollider::GetSize()
{
    return _size;
}

glm::vec3 BoxCollider::GetCenter()
{
    return _center;
}