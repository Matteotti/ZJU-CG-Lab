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
    float _dynamicFriction;
    float _staticFriction;
    CombineMode _bouncinessCombine;
    CombineMode _frictionCombine;
};