#include "BoxCollider.h"

#include "SphereCollider.h"
#include "Tools/Tools.h"

BoxCollider::BoxCollider()
{
    _colliderType = ColliderType::Box;
    _size = glm::vec3(1.0f);
    _center = glm::vec3(0.0f);
    UpdateVertices();
    UpdateAxis();
}

void BoxCollider::SetSize(glm::vec3 size)
{
    _size = size;
    UpdateVertices();
    UpdateAxis();
}

void BoxCollider::SetCenter(glm::vec3 center)
{
    _center = center;
    UpdateVertices();
    UpdateAxis();
}

glm::vec3 BoxCollider::GetSize()
{
    return _size;
}

glm::vec3 BoxCollider::GetCenter()
{
    return _center;
}

std::vector<glm::vec3> BoxCollider::GetVertices()
{
    return _vertices;
}

std::vector<glm::vec3> BoxCollider::GetAxis()
{
    return _axis;
}

void BoxCollider::UpdateVertices()
{
    _vertices.clear();
    _vertices.push_back(glm::vec3(_center.x - _size.x / 2.0f, _center.y - _size.y / 2.0f, _center.z - _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x + _size.x / 2.0f, _center.y - _size.y / 2.0f, _center.z - _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x - _size.x / 2.0f, _center.y + _size.y / 2.0f, _center.z - _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x + _size.x / 2.0f, _center.y + _size.y / 2.0f, _center.z - _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x - _size.x / 2.0f, _center.y - _size.y / 2.0f, _center.z + _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x + _size.x / 2.0f, _center.y - _size.y / 2.0f, _center.z + _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x - _size.x / 2.0f, _center.y + _size.y / 2.0f, _center.z + _size.z / 2.0f));
    _vertices.push_back(glm::vec3(_center.x + _size.x / 2.0f, _center.y + _size.y / 2.0f, _center.z + _size.z / 2.0f));
}

void BoxCollider::UpdateAxis()
{
    _axis.clear();
    _axis.push_back(ToolFunctions::NormalVector(_vertices[0], _vertices[1], _vertices[2]));
    _axis.push_back(ToolFunctions::NormalVector(_vertices[0], _vertices[1], _vertices[4]));
    _axis.push_back(ToolFunctions::NormalVector(_vertices[0], _vertices[2], _vertices[4]));
}

// using SAT (Separating Axis Theorem) to check collision
bool BoxCollider::CheckCollision(std::shared_ptr<Collider> other)
{
    ColliderType otherType = other->GetColliderType();
    switch (otherType)
    {
    case Box:
        for (int i = 0; i < _axis.size(); i++)
        {
            std::vector<glm::vec3> boxVertices1 = GetVertices();
            std::vector<glm::vec3> boxVertices2 = std::dynamic_pointer_cast<BoxCollider>(other)->GetVertices();
            std::vector<glm::vec3> allVertices = GetVertices();
            allVertices.insert(allVertices.end(), std::dynamic_pointer_cast<BoxCollider>(other)->GetVertices().begin(),
                               std::dynamic_pointer_cast<BoxCollider>(other)->GetVertices().end());

            float boxProjectionLength1 = ToolFunctions::GetProjectionLength(_axis[i], boxVertices1);
            float boxProjectionLength2 = ToolFunctions::GetProjectionLength(_axis[i], boxVertices2);

            float allProjectionLength = ToolFunctions::GetProjectionLength(_axis[i], allVertices);

            if (boxProjectionLength1 + boxProjectionLength2 < allProjectionLength)
            {
                return false;
            }
        }
        return true;
        break;

    case Sphere:
        for (int i = 0; i < _axis.size(); i++)
        {
            std::vector<glm::vec3> boxVertices = GetVertices();
            std::vector<glm::vec3> sphereVertices = ToolFunctions::GetSphereProjectionVertices(
                std::dynamic_pointer_cast<SphereCollider>(other)->GetCenter(),
                std::dynamic_pointer_cast<SphereCollider>(other)->GetRadius(), _axis[i]);
            std::vector<glm::vec3> allVertices = GetVertices();
            allVertices.insert(allVertices.end(), sphereVertices.begin(), sphereVertices.end());

            float boxProjectionLength = ToolFunctions::GetProjectionLength(_axis[i], boxVertices);
            float sphereProjectionLength = ToolFunctions::GetProjectionLength(_axis[i], sphereVertices);

            float allProjectionLength = ToolFunctions::GetProjectionLength(_axis[i], allVertices);

            if (boxProjectionLength + sphereProjectionLength < allProjectionLength)
            {
                return false;
            }
        }
        return true;
        break;

    default:
        return false;
    }
}