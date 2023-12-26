#include "BoxCollider.h"

#include "SphereCollider.h"
#include "Tools/Tools.h"

BoxCollider::BoxCollider()
{
    _horizontal = glm::vec3(1.0f, 0.0f, 0.0f);
    _vertical = glm::vec3(0.0f, 1.0f, 0.0f);
    _colliderType = ColliderType::Box;
    _horizontal = glm::vec3(1.0f);
    _vertical = glm::vec3(1.0f);
    _depth = glm::vec3(1.0f);
    _center = glm::vec3(0.0f);
    UpdateVertices();
    UpdateAxis();
}

void BoxCollider::SetSize(glm::vec3 size)
{
    _horizontal = glm::normalize(_horizontal) * size.x;
    _vertical = glm::normalize(_vertical) * size.y;
    _depth = glm::normalize(_depth) * size.z;
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
    return glm::vec3(glm::length(_horizontal), glm::length(_vertical), glm::length(_depth));
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
    _vertices.push_back(_center - _horizontal - _vertical - _depth);
    _vertices.push_back(_center + _horizontal - _vertical - _depth);
    _vertices.push_back(_center - _horizontal + _vertical - _depth);
    _vertices.push_back(_center + _horizontal + _vertical - _depth);
    _vertices.push_back(_center - _horizontal - _vertical + _depth);
    _vertices.push_back(_center + _horizontal - _vertical + _depth);
    _vertices.push_back(_center - _horizontal + _vertical + _depth);
    _vertices.push_back(_center + _horizontal + _vertical + _depth);
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

void BoxCollider::Update()
{
    glm::mat4 trans = glm::mat4(1.0f);
    glm::scale(trans, _scale);
    trans = glm::rotate(trans, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _horizontal = trans * glm::vec4(_horizontal, 1.0f);
    _vertical = trans * glm::vec4(_vertical, 1.0f);
    _depth = trans * glm::vec4(_depth, 1.0f);
    UpdateVertices();
    UpdateAxis();
}