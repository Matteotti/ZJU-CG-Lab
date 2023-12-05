#include "SphereCollider.h"

#include <vector>

SphereCollider::SphereCollider()
{
    _colliderType = ColliderType::Sphere;
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

bool SphereCollider::CheckCollision(std::shared_ptr<Collider> other)
{
    ColliderType otherType = other->GetColliderType();

    std::vector<glm::vec3> axis = std::vector<glm::vec3>();

    float radius1 = 0.0f;
    float radius2 = 0.0f;

    glm::vec3 center1 = glm::vec3(0.0f);
    glm::vec3 center2 = glm::vec3(0.0f);

    float distance = 0.0f;

    switch (otherType)
    {
    case Box:
        axis = std::dynamic_pointer_cast<BoxCollider>(other)->GetAxis();

        for (int i = 0; i < axis.size(); i++)
        {
            std::vector<glm::vec3> boxVertices = std::dynamic_pointer_cast<BoxCollider>(other)->GetVertices();
            std::vector<glm::vec3> sphereVertices = ToolFunctions::GetSphereProjectionVertices(_center, _radius, axis[i]);
            std::vector<glm::vec3> allVertices = boxVertices;
            allVertices.insert(allVertices.end(), sphereVertices.begin(), sphereVertices.end());

            float boxProjectionLength = ToolFunctions::GetProjectionLength(axis[i], boxVertices);
            float sphereProjectionLength = ToolFunctions::GetProjectionLength(axis[i], sphereVertices);

            float allProjectionLength = ToolFunctions::GetProjectionLength(axis[i], allVertices);

            if (boxProjectionLength + sphereProjectionLength <= allProjectionLength)
            {
                return false;
            }
        }
        return true;
        break;

    case Sphere:
        radius1 = _radius;
        radius2 = std::dynamic_pointer_cast<SphereCollider>(other)->GetRadius();

        center1 = _center;
        center2 = std::dynamic_pointer_cast<SphereCollider>(other)->GetCenter();

        distance = glm::distance(center1, center2);

        if (distance <= radius1 + radius2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    default:
        return false;
    }
}