#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class Transform : public std::enable_shared_from_this<Transform>
{
public:
    Transform();
    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetForwardDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetUpDirection() const;
    glm::vec3 GetScale() const;
    glm::mat4 GetModelMatrix() const;

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    void Translate(glm::vec3 translation);
    void Rotate(glm::vec3 rotation);
    void Rotate(float angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);

    std::shared_ptr<Transform> GetParent() const;
    std::shared_ptr<Transform> GetFirstChild() const;
    std::shared_ptr<Transform> GetPrevSibling() const;
    std::shared_ptr<Transform> GetNextSibling() const;
    int GetChildCount() const;

    void SetParent(std::shared_ptr<Transform> parent, int index = 0);
    void RemoveParent();

private:
    glm::vec3 _position;
    // in degree
    glm::vec3 _rotation;
    glm::vec3 _scale;

    glm::mat4 _translationMatrix;
    glm::mat4 _rotationMatrix;
    glm::mat4 _scaleMatrix;
    glm::mat4 _modelMatrix;

    int _childCount;
    std::shared_ptr<Transform> _parent;
    std::shared_ptr<Transform> _firstChild;
    std::shared_ptr<Transform> _prevSibling;
    std::shared_ptr<Transform> _nextSibling;
};