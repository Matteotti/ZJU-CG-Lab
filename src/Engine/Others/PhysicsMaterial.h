#pragma once
enum CombineMode
{
    Average,
    Minimum,
    Multiply,
    Maximum
};
// priority: average < minimum < multiply < maximum
class PhysicsMaterial
{
private:
    float _bounciness;
    float _friction;
    CombineMode _bouncinessCombine;
    CombineMode _frictionCombine;

public:
    PhysicsMaterial(float bounciness = 0.6f, float friction = 0.4f, CombineMode bouncinessCombine = Average,
                    CombineMode frictionCombine = Average);

    float CombinedBounciness(PhysicsMaterial &otherMaterial) const;
    float CombinedFriction(PhysicsMaterial &otherMaterial) const;
};