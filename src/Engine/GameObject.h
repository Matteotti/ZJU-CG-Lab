#pragma once
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Tags.h"

int g_minAvailuableID = 0;

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

std::map<int, GameObject *> g_gameObjects;

std::multimap<int, int> g_disableZBufferList;

std::multimap<int, int> g_enableZBufferList;