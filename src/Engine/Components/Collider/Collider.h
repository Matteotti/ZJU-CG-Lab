#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "Others/Layer.h"
#include "Others/PhysicsMaterial.h"

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

public:
    Collider();

    void SetEnabled(bool isEnabled);
    void SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial);
    void SetLayer(Layer layer);
    void SetLayerMask(LayerMask layerMask);

    ColliderType GetColliderType();

    virtual bool CheckCollision(std::shared_ptr<Collider> other) = 0;

    bool IsEnabled();
    std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial();
    Layer GetLayer();
    LayerMask GetLayerMask();
};