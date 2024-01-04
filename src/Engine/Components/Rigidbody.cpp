#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
    _mass = 1.0f;
    _inertiaTensor = 1.0f;

    _angularVelocity = glm::vec3(0.0f);
    _velocity = glm::vec3(0.0f);

    _angularDrag = 0.0f;
    _drag = 0.0f;

    _maxAngularVelocity = 7.0f;
    _maxVelocity = 7.0f;

    _useGravity = true;
    _gravity = glm::vec3(0.0f, -9.8f, 0.0f);
}

/// @brief Add force to the Rigidbody
/// @param force The force to add
/// @param time The time to apply the force
void Rigidbody::AddForce(glm::vec3 force, float time)
{
    _forces.push_back(force);
    _forcesPosition.push_back(glm::vec3(0.0f));
    _forcesTime.push_back(time);
}

/// @brief Add force to the Rigidbody at a specific position
/// @param force The force to add
/// @param position The position to apply the force in world space
/// @param time The time to apply the force
void Rigidbody::AddForceAtPosition(glm::vec3 force, glm::vec3 position, float time)
{
    _forces.push_back(force);
    _forcesPosition.push_back(position);
    _forcesTime.push_back(time);
}

/// @brief Add relative force to the Rigidbody
/// @param force The force to add
/// @param position The position to apply the force in local space
/// @param time The time to apply the force
void Rigidbody::AddRelativeForce(glm::vec3 force, glm::vec3 position, float time)
{
    _relativeForces.push_back(force);
    _rawRelativeForcesPosition.push_back(position);
    _relativeForcesPosition.push_back(position);
    _relativeForcesTime.push_back(time);
}

/// @brief Add impulse to the Rigidbody
/// @param impulse The impulse to add
/// @param position The position to apply the force
void Rigidbody::AddImpulse(glm::vec3 impulse, glm::vec3 position)
{
    _impulse.push_back(impulse);
    _impulsePosition.push_back(position);
}

/// @brief Update the relative forces position according to rotation
/// @param rotation The rotation to update
void Rigidbody::UpdateRelativeForcesPosition(glm::vec3 rotation, glm::vec3 scale)
{
    _relativeForcesPosition.clear();
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, scale);
    for (auto &position : _rawRelativeForcesPosition)
    {
        position = trans * glm::vec4(position, 1.0f);
        _relativeForcesPosition.push_back(position);
    }
}

void Rigidbody::SetMass(float mass)
{
    _mass = mass;
}

void Rigidbody::SetInertiaTensor(float inertiaTensor)
{
    _inertiaTensor = inertiaTensor;
}

void Rigidbody::SetAngularVelocity(glm::vec3 angularVelocity)
{
    _angularVelocity = angularVelocity;
}

void Rigidbody::SetVelocity(glm::vec3 velocity)
{
    _velocity = velocity;
}

void Rigidbody::SetAngularDrag(float angularDrag)
{
    _angularDrag = angularDrag;
}

void Rigidbody::SetDrag(float drag)
{
    _drag = drag;
}

void Rigidbody::SetMaxAngularVelocity(float maxAngularVelocity)
{
    _maxAngularVelocity = maxAngularVelocity;
}

void Rigidbody::SetMaxVelocity(float maxVelocity)
{
    _maxVelocity = maxVelocity;
}

void Rigidbody::SetIsUseGravity(bool useGravity)
{
    _useGravity = useGravity;
}

void Rigidbody::SetGravity(glm::vec3 gravity)
{
    _gravity = gravity;
}

float Rigidbody::GetMass() const
{
    return _mass;
}

float Rigidbody::GetInertiaTensor() const
{
    return _inertiaTensor;
}

glm::vec3 Rigidbody::GetAngularVelocity() const
{
    return _angularVelocity;
}

glm::vec3 Rigidbody::GetVelocity() const
{
    return _velocity;
}

float Rigidbody::GetAngularDrag() const
{
    return _angularDrag;
}

float Rigidbody::GetDrag() const
{
    return _drag;
}

float Rigidbody::GetMaxAngularVelocity() const
{
    return _maxAngularVelocity;
}

float Rigidbody::GetMaxVelocity() const
{
    return _maxVelocity;
}

bool Rigidbody::IsUseGravity() const
{
    return _useGravity;
}

glm::vec3 Rigidbody::GetGravity() const
{
    return _gravity;
}

std::vector<glm::vec3> &Rigidbody::GetForces()
{
    return _forces;
}

std::vector<glm::vec3> &Rigidbody::GetForcesPosition()
{
    return _forcesPosition;
}

std::vector<float> &Rigidbody::GetForcesTime()
{
    return _forcesTime;
}

std::vector<glm::vec3> &Rigidbody::GetRelativeForces()
{
    return _relativeForces;
}

std::vector<glm::vec3> &Rigidbody::GetRelativeForcesPosition()
{
    return _relativeForcesPosition;
}

std::vector<float> &Rigidbody::GetRelativeForcesTime()
{
    return _relativeForcesTime;
}

std::vector<glm::vec3> &Rigidbody::GetImpulse()
{
    return _impulse;
}

std::vector<glm::vec3> &Rigidbody::GetImpulsePosition()
{
    return _impulsePosition;
}