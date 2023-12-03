#include "GameObject.h"

GameObject::GameObject()
{
    IsHidingFlag = false;
    Tag = DEFAULT_TAG;
    Layer = 0b0;
    Name = "GameObject";

    _isUsingZBufferFlag = false;
    _sortingOrder = 0;
    AssignID();
    _childCount = 0;
    _parentID = -1;
    _firstChildrenID = -1;
    _prevSiblingID = -1;
    _nextSiblingID = -1;
    _position = glm::vec3(0.0f);
    _rotation = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);
    _translateMatrix = glm::mat4(1.0f);
    _rotateMatrix = glm::mat4(1.0f);
    _scaleMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::mat4(1.0f);

    // TODO: empty, 因为我还不太清楚modern opengl的渲染流程
    _mesh = nullptr;
    _texture = nullptr;
    _material = nullptr;
    // ...

    g_gameObjects.insert(std::pair<int, GameObject *>(_id, this));
    g_enableZBufferList.insert(std::pair<int, int>(_sortingOrder, _id));
}

GameObject::GameObject(std::string name, Mesh *mesh, Texture *texture, Material *material)
{
    IsHidingFlag = false;
    Tag = DEFAULT_TAG;
    Layer = 0b0;
    Name = name;

    _isUsingZBufferFlag = false;
    _sortingOrder = 0;
    AssignID();
    _childCount = 0;
    _parentID = -1;
    _firstChildrenID = -1;
    _prevSiblingID = -1;
    _nextSiblingID = -1;
    _position = glm::vec3(0.0f);
    _rotation = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);
    _translateMatrix = glm::mat4(1.0f);
    _rotateMatrix = glm::mat4(1.0f);
    _scaleMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::mat4(1.0f);

    // TODO: empty, 因为我还不太清楚modern opengl的渲染流程
    _mesh = mesh;
    _texture = texture;
    _material = material;
    // ...

    g_gameObjects.insert(std::pair<int, GameObject *>(_id, this));
    g_enableZBufferList.insert(std::pair<int, int>(_sortingOrder, _id));
}

GameObject::~GameObject()
{
    delete _mesh;
    delete _texture;
    delete _material;

    g_gameObjects.erase(_id);

    // FIXME Time complexity is O(n)!
    for (const auto &it : g_disableZBufferList)
    {
        if (it.second == _id)
        {
            g_disableZBufferList.erase(it.first);
            break;
        }
    }
}

bool GameObject::GetUsingZBufferFlag()
{
    return _isUsingZBufferFlag;
}

int GameObject::GetSortingOrder()
{
    return _sortingOrder;
}

int GameObject::GetID()
{
    return _id;
}

int GameObject::GetChildCount()
{
    return _childCount;
}

GameObject *GameObject::GetParent()
{
    if (_parentID == -1)
    {
        return nullptr;
    }
    return g_gameObjects[_parentID];
}

GameObject *GameObject::GetFirstChild()
{
    if (_firstChildrenID == -1)
    {
        return nullptr;
    }
    return g_gameObjects[_firstChildrenID];
}

GameObject *GameObject::GetPrevSibling()
{
    if (_prevSiblingID == -1)
    {
        return nullptr;
    }
    return g_gameObjects[_prevSiblingID];
}

GameObject *GameObject::GetNextSibling()
{
    if (_nextSiblingID == -1)
    {
        return nullptr;
    }
    return g_gameObjects[_nextSiblingID];
}

glm::vec3 GameObject::GetPosition()
{
    return _position;
}

glm::vec3 GameObject::GetRotation()
{
    return _rotation;
}

glm::vec3 GameObject::GetScale()
{
    return _scale;
}

void GameObject::SetZBuffer(bool isUsingZBufferFlag)
{
    _isUsingZBufferFlag = isUsingZBufferFlag;
    // FIXME Time complexity is O(n)!
    for (const auto &it : g_disableZBufferList)
    {
        if (it.second == _id)
        {
            // found current object in disable_z_buffer_list
            if (_isUsingZBufferFlag)
            {
                // if current object is using z buffer, then remove it from disable_z_buffer_list
                g_disableZBufferList.erase(it.first);
            }
            else
            {
                // if current object is not using z buffer, then update
                g_disableZBufferList.erase(it.first);
                g_disableZBufferList.insert(std::pair<int, int>(_sortingOrder, _id));
            }
        }
    }
    // FIXME Time complexity is O(n)!
    for (const auto &it : g_enableZBufferList)
    {
        if (it.second == _id)
        {
            // found current object in enable_z_buffer_list
            if (_isUsingZBufferFlag)
            {
                // if current object is using z buffer, then update
                g_enableZBufferList.erase(it.first);
                g_enableZBufferList.insert(std::pair<int, int>(_sortingOrder, _id));
            }
            else
            {
                // if current object is not using z buffer, then remove it from enable_z_buffer_list
                g_enableZBufferList.erase(it.first);
            }
        }
    }
}

void GameObject::SetSortingOrder(int sortingOrder)
{
    _sortingOrder = sortingOrder;
    // store object in disable_z_buffer_list, auto sorting according to sortingOrder
    if (!_isUsingZBufferFlag)
    {
        for (const auto &it : g_disableZBufferList)
        {
            if (it.second == _id)
            {
                g_disableZBufferList.erase(it.first);
                g_disableZBufferList.insert(std::pair<int, int>(_sortingOrder, _id));
                break;
            }
        }
    }
}

// once the ID is assigned, it cannot be changed
void GameObject::AssignID()
{
    _id = g_minAvailuableID++;
}

/// @brief set parent of current object
/// @param parent  parent object
/// @param index  index of current object in parent's children list
void GameObject::SetParent(GameObject *parent, int index)
{
    // if parent is nullptr, return
    if (parent == nullptr)
    {
        _parentID = -1;
        return;
    }
    // if parent is current object's child, return
    GameObject *m_protentialParent = parent;

    // FIXME: Time complexity is O(n)!
    while (m_protentialParent != nullptr)
    {
        if (m_protentialParent == this)
        {
            return;
        }
        m_protentialParent = m_protentialParent->GetParent();
    }

    // if parent is not nullptr and parent is not current object's child
    // then remove current parent before assigning new parent
    RemoveParent();
    _parentID = parent->GetID();
    GameObject *m_currentAcessedChild = parent->GetFirstChild();
    GameObject *m_prevAcessedChild = parent->GetFirstChild();
    // index beyond the range of children list
    if (index > parent->GetChildCount() - 1)
    {
        while (m_currentAcessedChild != nullptr)
        {
            m_prevAcessedChild = m_currentAcessedChild;
            m_currentAcessedChild = m_currentAcessedChild->GetNextSibling();
        }
        if (m_prevAcessedChild == nullptr)
        {
            parent->_firstChildrenID = _id;
            _prevSiblingID = -1;
            _nextSiblingID = -1;
        }
        else
        {
            m_prevAcessedChild->_nextSiblingID = _id;
            _prevSiblingID = m_prevAcessedChild->GetID();
        }
    }
    // index in the range of children list
    else
    {
        for (int i = 0; i < index; i++)
        {
            m_prevAcessedChild = m_currentAcessedChild;
            m_currentAcessedChild = m_currentAcessedChild->GetNextSibling();
        }
        // if current object is the first child
        if (index == 0)
        {
            parent->_firstChildrenID = _id;
            _prevSiblingID = -1;
            _nextSiblingID = m_currentAcessedChild->GetID();
        }
        else
        {
            m_prevAcessedChild->_nextSiblingID = _id;
            m_currentAcessedChild->_prevSiblingID = _id;
            _prevSiblingID = m_prevAcessedChild->GetID();
            _nextSiblingID = m_currentAcessedChild->GetID();
        }
    }
}

void GameObject::RemoveParent()
{
    // if current object has no parent, return
    if (_parentID == -1)
    {
        return;
    }
    // if current object has parent
    GameObject *m_parent = GetParent();
    // if current object is the first child
    if (m_parent->GetFirstChild() == this)
    {
        m_parent->_firstChildrenID = _nextSiblingID;
        if (_nextSiblingID != -1)
        {
            GameObject *m_nextSibling = m_parent->GetNextSibling();
            m_nextSibling->_prevSiblingID = -1;
        }
    }
    // if current object is not the first child
    else
    {
        GameObject *m_prevSibling = GetPrevSibling();
        GameObject *m_nextSibling = GetNextSibling();
        m_prevSibling->_nextSiblingID = _nextSiblingID;
        if (_nextSiblingID != -1)
        {
            m_nextSibling->_prevSiblingID = _prevSiblingID;
        }
    }
    _parentID = -1;
    _prevSiblingID = -1;
    _nextSiblingID = -1;
}

void GameObject::SetPosition(glm::vec3 position)
{
    _translateMatrix = glm::mat4(1.0f);
    _translateMatrix = glm::translate(_translateMatrix, position);
    _modelMatrix = _translateMatrix * _rotateMatrix * _scaleMatrix;
}

void GameObject::SetRotation(glm::vec3 rotation)
{
    _rotateMatrix = glm::mat4(1.0f);
    _rotateMatrix = glm::rotate(_rotateMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _rotateMatrix = glm::rotate(_rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _rotateMatrix = glm::rotate(_rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _modelMatrix = _translateMatrix * _rotateMatrix * _scaleMatrix;
}

void GameObject::SetScale(glm::vec3 scale)
{
    _scaleMatrix = glm::mat4(1.0f);
    _scaleMatrix = glm::scale(_scaleMatrix, scale);
    _modelMatrix = _translateMatrix * _rotateMatrix * _scaleMatrix;
}

bool GameObject::operator==(const GameObject &other) const
{
    return _id == other._id;
}