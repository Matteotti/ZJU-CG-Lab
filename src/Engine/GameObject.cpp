#include "GameObject.h"

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tags.h"

class GameObject
{
public:
    bool IsHidingFlag = false;
    int Tag = DEFAULT_TAG; // be used for collision comparison
    int32_t Layer = 0b0;   // be used for collision ignorance
    std::string Name = "GameObject";

    GameObject();
    GameObject(std::string name, Mesh *mesh, Texture *texture, Material *material);
    ~GameObject();

    bool GetUsingZBufferFlag();
    int GetSortingOrder();
    int GetID();
    int GetChildCount();
    GameObject *GetParent();
    GameObject *GetFirstChild();
    GameObject *GetPrevSibling();
    GameObject *GetNextSibling();
    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();

    void SetZBuffer(bool isUsingZBufferFlag);
    void SetSortingOrder(int sortingOrder);
    // once the ID is assigned, it cannot be changed
    void AssignID();
    void SetParent(GameObject *parent, int index = 0);
    void RemoveParent();
    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    bool operator==(const GameObject &other) const;

private:
    bool _isUsingZBufferFlag = false; // set to true if you want to render this object first
                                      // if this is true, then the object will be rendered using "Disable GL_DEPTH_TEST"
    int _sortingOrder = 0;            // small number means render first, used only when _isUsingZBufferFlag is true
    int _id = -1;
    int _childCount;
    int _parentID = -1;
    int _firstChildrenID = -1;
    int _prevSiblingID = -1;
    int _nextSiblingID = -1;
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _rotation = glm::vec3(0.0f); // Euler Angles, x, y, z in degree
    glm::vec3 _scale = glm::vec3(1.0f);
    glm::mat4 _modelMatrix = glm::mat4(1.0f);

    // TODO: empty, 因为我还不太清楚modern opengl的渲染流程
    Mesh *_mesh;
    Texture *_texture;
    Material *_material;
    // ...
};

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
void GameObject::SetParent(GameObject *parent, int index = 0)
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
    _modelMatrix = glm::translate(_modelMatrix, -_position);
    _position = position;
    _modelMatrix = glm::translate(_modelMatrix, position);
}

void GameObject::SetRotation(glm::vec3 rotation)
{
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(-_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(-_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(-_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _rotation = rotation;
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void GameObject::SetScale(glm::vec3 scale)
{
    _modelMatrix = glm::scale(_modelMatrix, 1.0f / _scale);
    _scale = scale;
    _modelMatrix = glm::scale(_modelMatrix, scale);
}

bool GameObject::operator==(const GameObject &other) const
{
    return _id == other._id;
}