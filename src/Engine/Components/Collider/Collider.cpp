#include "Collider.h"

void Collider::SetEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

void Collider::SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial)
{
    _physicsMaterial = physicsMaterial;
}

void Collider::SetLayer(Layer layer)
{
    _layer = layer;
}

void Collider::SetLayerMask(LayerMask layerMask)
{
    _layerMask = layerMask;
}

bool Collider::IsEnabled()
{
    return _isEnabled;
}

std::shared_ptr<PhysicsMaterial> Collider::GetPhysicsMaterial()
{
    return _physicsMaterial;
}

Layer Collider::GetLayer()
{
    return _layer;
}

LayerMask Collider::GetLayerMask()
{
    return _layerMask;
}

ColliderType Collider::GetColliderType()
{
    return _colliderType;
}

Collider::Collider()
{
    _colliderType = ColliderType::Other;
    _isEnabled = true;
    _physicsMaterial = std::make_shared<PhysicsMaterial>();
    _layer = Layers::Default;
    _layerMask = LayerMasks::All;
}