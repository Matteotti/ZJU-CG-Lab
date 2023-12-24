#pragma once

#include "Others/Layer.h"
#include "Others/PhysicsMaterial.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

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

protected:
    ColliderType _colliderType;

    glm::vec3 _rotation;
    glm::vec3 _scale;

public:
    Collider();

    void SetEnabled(bool isEnabled);
    void SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial);
    void SetLayer(Layer layer);
    void SetLayerMask(LayerMask layerMask);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    ColliderType GetColliderType();

    virtual bool CheckCollision(std::shared_ptr<Collider> other) = 0;
    virtual void Update() = 0;

    bool IsEnabled();
    std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial();
    Layer GetLayer();
    LayerMask GetLayerMask();
};