#include "Transform.h"

Transform::Transform()
{
    _position = glm::vec3(0.0f);
    _rotation = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);

    _translationMatrix = glm::mat4(1.0f);
    _rotationMatrix = glm::mat4(1.0f);
    _scaleMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::mat4(1.0f);

    _childCount = 0;
    _parent = nullptr;
    _firstChild = nullptr;
    _prevSibling = nullptr;
    _nextSibling = nullptr;
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    _position = position;
    _rotation = rotation;
    _scale = scale;

    _translationMatrix = glm::translate(glm::mat4(1.0f), _position);
    _rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;

    _childCount = 0;
    _parent = nullptr;
    _firstChild = nullptr;
    _prevSibling = nullptr;
    _nextSibling = nullptr;
}

glm::vec3 Transform::GetPosition() const
{
    return _position;
}

glm::vec3 Transform::GetRotation() const
{
    return _rotation;
}

glm::vec3 Transform::GetScale() const
{
    return _scale;
}

glm::mat4 Transform::GetModelMatrix() const
{
    return _modelMatrix;
}

void Transform::SetPosition(glm::vec3 position)
{
    _position = position;
    _translationMatrix = glm::translate(glm::mat4(1.0f), _position);
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::SetRotation(glm::vec3 rotation)
{
    _rotation = rotation;
    _rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::SetScale(glm::vec3 scale)
{
    _scale = scale;
    _scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::Translate(glm::vec3 translation)
{
    _position += translation;
    _translationMatrix = glm::translate(glm::mat4(1.0f), _position);
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::Rotate(glm::vec3 rotation)
{
    _rotation += rotation;
    _rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle), axis);
    _rotation = glm::eulerAngles(glm::quat(_rotationMatrix));
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void Transform::Scale(glm::vec3 scale)
{
    _scale *= scale;
    _scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

std::shared_ptr<Transform> Transform::GetParent() const
{
    return _parent;
}

std::shared_ptr<Transform> Transform::GetFirstChild() const
{
    return _firstChild;
}

std::shared_ptr<Transform> Transform::GetPrevSibling() const
{
    return _prevSibling;
}

std::shared_ptr<Transform> Transform::GetNextSibling() const
{
    return _nextSibling;
}

int Transform::GetChildCount() const
{
    return _childCount;
}

/// @brief remove parent from current object
/// @param parent parent of current object
/// @param index index of current object in parent's children list
void Transform::SetParent(std::shared_ptr<Transform> parent, int index)
{
    // if parent pointer is nullptr, return
    if (!parent)
    {
        return;
    }
    // if parent is current object's child, return
    std::shared_ptr<Transform> potentialParent = parent;

    while (potentialParent != nullptr)
    {
        if (potentialParent.get() == this)
        {
            return;
        }
        potentialParent = potentialParent->GetParent();
    }

    // remove before set
    RemoveParent();

    // set parent
    _parent = parent;
    _parent->_childCount++;
    std::shared_ptr<Transform> currentAccessedChild = parent->GetFirstChild();
    std::shared_ptr<Transform> prevAccessedChild = parent->GetFirstChild();
    // index beyond the range of children list
    if (index > parent->GetChildCount() - 1)
    {
        while (currentAccessedChild)
        {
            prevAccessedChild = currentAccessedChild;
            currentAccessedChild = currentAccessedChild->GetNextSibling();
        }
        if (!prevAccessedChild)
        {
            parent->_firstChild = shared_from_this();
            _prevSibling.reset();
            _nextSibling.reset();
        }
        else
        {
            prevAccessedChild->_nextSibling = shared_from_this();
            _prevSibling = prevAccessedChild;
        }
    }
    // index in the range of children list
    else
    {
        for (int i = 0; i < index; i++)
        {
            prevAccessedChild = currentAccessedChild;
            currentAccessedChild = currentAccessedChild->GetNextSibling();
        }
        // if current object is the first child
        if (index == 0)
        {
            parent->_firstChild = shared_from_this();
            _prevSibling.reset();
            _nextSibling = currentAccessedChild;
        }
        else
        {
            prevAccessedChild->_nextSibling = shared_from_this();
            _prevSibling = prevAccessedChild;
            _nextSibling = currentAccessedChild;
            currentAccessedChild->_prevSibling = shared_from_this();
        }
    }
}

void Transform::RemoveParent()
{
    // if current object has no parent, return
    if (!_parent)
    {
        return;
    }
    // if current object has parent
    std::shared_ptr<Transform> parent = GetParent();
    parent->_childCount--;
    // if current object is the first child
    if (parent->_firstChild == shared_from_this())
    {
        parent->_firstChild = _nextSibling;
        if (_nextSibling != nullptr)
        {
            _nextSibling->_prevSibling.reset();
        }
    }
    // if current object is not the first child
    else
    {
        _prevSibling->_nextSibling = _nextSibling;
        if (_nextSibling != nullptr)
        {
            _nextSibling->_prevSibling = _prevSibling;
        }
    }
    _parent.reset();
    _prevSibling.reset();
    _nextSibling.reset();
}