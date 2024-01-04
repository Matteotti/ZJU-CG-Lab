#pragma once

#include "Others/CollisionInfo.h"
#include "Others/Layer.h"
#include "Others/PhysicsMaterial.h"
#include "Tools/Tools.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>

enum ColliderType
{
    Box,
    Sphere,
    Other
};

class Collider
{
private:
    bool _isEnabled;

    std::shared_ptr<PhysicsMaterial> _physicsMaterial;

    // current object's layer
    Layer _layer;
    // layer mask for collision detection
    LayerMask _layerMask;

    ColliderType _colliderType;

    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

    float PointToPlane(glm::vec3 point, glm::vec4 plane);

#pragma region Box

    glm::vec3 _boxCenter;
    glm::vec3 _horizontal;
    glm::vec3 _vertical;
    glm::vec3 _depth;
    glm::vec3 _rawHorizontal;
    glm::vec3 _rawVertical;
    glm::vec3 _rawDepth;

    std::vector<glm::vec3> _vertices;

    void UpdateBoxVertices();

#pragma endregion

#pragma region Sphere

    float _radius;
    glm::vec3 _sphereCenter;

#pragma endregion

public:
    Collider();
    Collider(ColliderType colliderType);

    void SetEnabled(bool isEnabled);
    void SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial);
    void SetLayer(Layer layer);
    void SetLayerMask(LayerMask layerMask);
    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    ColliderType GetColliderType();

    bool IsEnabled();
    std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial();
    Layer GetLayer();
    LayerMask GetLayerMask();
    glm::vec3 GetPosition();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();

    std::shared_ptr<CollisionInfo> CheckCollision(Collider &other);
    std::shared_ptr<CollisionInfo> BoxToBox(Collider &other);
    std::shared_ptr<CollisionInfo> BoxToSphere(Collider &other);
    std::shared_ptr<CollisionInfo> SphereToBox(Collider &other);
    std::shared_ptr<CollisionInfo> SphereToSphere(Collider &other);
    void Update();

#pragma region Box

    void SetBoxHalfSize(glm::vec3 size);
    void SetBoxCenter(glm::vec3 center);

    const std::vector<glm::vec3> &GetBoxVertices() const;

    glm::vec3 GetBoxSize() const;
    glm::vec3 GetBoxCenter() const;

#pragma endregion

#pragma region Sphere

    void SetSphereRadius(float radius);
    void SetSphereCenter(glm::vec3 center);

    float GetSphereRadius() const;
    glm::vec3 GetSphereCenter() const;

#pragma endregion
};