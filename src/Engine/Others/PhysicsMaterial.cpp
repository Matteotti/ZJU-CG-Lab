#include "PhysicsMaterial.h"
#include <utility>

PhysicsMaterial::PhysicsMaterial(float bounciness, float friction, CombineMode bouncinessCombine,
                                 CombineMode frictionCombine)
    : _bounciness(bounciness), _friction(friction), _bouncinessCombine(bouncinessCombine),
      _frictionCombine(frictionCombine)
{
}

float PhysicsMaterial::CombinedBounciness(PhysicsMaterial &otherMaterial) const
{
    switch (_bouncinessCombine)
    {
    case Average:
        return (_bounciness + otherMaterial._bounciness) / 2;
    case Minimum:
        return std::min(_bounciness, otherMaterial._bounciness);
    case Multiply:
        return _bounciness * otherMaterial._bounciness;
    case Maximum:
        return std::max(_bounciness, otherMaterial._bounciness);
    default:
        return 0;
    }
}

float PhysicsMaterial::CombinedFriction(PhysicsMaterial &otherMaterial) const
{
    switch (_frictionCombine)
    {
    case Average:
        return (_friction + otherMaterial._friction) / 2;
    case Minimum:
        return std::min(_friction, otherMaterial._friction);
    case Multiply:
        return _friction * otherMaterial._friction;
    case Maximum:
        return std::max(_friction, otherMaterial._friction);
    default:
        return 0;
    }
}