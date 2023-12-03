#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
    _mass = 1.0f;

    _angularVelocity = glm::vec3(0.0f);
    _velocity = glm::vec3(0.0f);

    _angularDrag = 0.0f;
    _drag = 0.0f;

    _maxAngularVelocity = 7.0f;
    _maxVelocity = 7.0f;

    _useGravity = true;
    _gravity = glm::vec3(0.0f, -9.8f, 0.0f);

    _layer = Layers::Default;
    _layerMask = LayerMasks::All;
}

void Rigidbody::OnCollisionEnter(std::shared_ptr<Rigidbody> other)
{
    std::cout << "OnCollisionEnter" << std::endl;
    // TODO: Implement this
}

void Rigidbody::OnCollisionExit(std::shared_ptr<Rigidbody> other)
{
    std::cout << "OnCollisionExit" << std::endl;
    // TODO: Implement this
}

void Rigidbody::OnCollisionStay(std::shared_ptr<Rigidbody> other)
{
    std::cout << "OnCollisionStay" << std::endl;
    // TODO: Implement this
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

/// @brief Add torque to the Rigidbody
/// @param torque The torque to add
/// @param time The time to apply the torque
void Rigidbody::AddTorque(glm::vec3 torque, float time)
{
    _torques.push_back(torque);
    _torquesTime.push_back(time);
}

/// @brief Add relative force to the Rigidbody
/// @param force The force to add
/// @param position The position to apply the force in local space
/// @param time The time to apply the force
void Rigidbody::AddRelativeForce(glm::vec3 force, glm::vec3 position, float time)
{
    _relativeForces.push_back(force);
    _relativeForcesPosition.push_back(position);
    _relativeForcesTime.push_back(time);
}

/// @brief Add relative torque to the Rigidbody
/// @param torque The torque to add
/// @param time The time to apply the torque
void Rigidbody::AddRelativeTorque(glm::vec3 torque, float time)
{
    _relativeTorques.push_back(torque);
    _relativeTorquesTime.push_back(time);
}

void Rigidbody::SetMass(float mass)
{
    _mass = mass;
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

float Rigidbody::GetMass()
{
    return _mass;
}

glm::vec3 Rigidbody::GetAngularVelocity()
{
    return _angularVelocity;
}

glm::vec3 Rigidbody::GetVelocity()
{
    return _velocity;
}

float Rigidbody::GetAngularDrag()
{
    return _angularDrag;
}

float Rigidbody::GetDrag()
{
    return _drag;
}

float Rigidbody::GetMaxAngularVelocity()
{
    return _maxAngularVelocity;
}

float Rigidbody::GetMaxVelocity()
{
    return _maxVelocity;
}

bool Rigidbody::IsUseGravity()
{
    return _useGravity;
}

glm::vec3 Rigidbody::GetGravity()
{
    return _gravity;
}

std::list<glm::vec3> Rigidbody::GetForces()
{
    return _forces;
}

std::list<glm::vec3> Rigidbody::GetForcesPosition()
{
    return _forcesPosition;
}

std::list<float> Rigidbody::GetForcesTime()
{
    return _forcesTime;
}

std::list<glm::vec3> Rigidbody::GetRelativeForces()
{
    return _relativeForces;
}

std::list<glm::vec3> Rigidbody::GetRelativeForcesPosition()
{
    return _relativeForcesPosition;
}

std::list<float> Rigidbody::GetRelativeForcesTime()
{
    return _relativeForcesTime;
}

std::list<glm::vec3> Rigidbody::GetTorques()
{
    return _torques;
}

std::list<float> Rigidbody::GetTorquesTime()
{
    return _torquesTime;
}

std::list<glm::vec3> Rigidbody::GetRelativeTorques()
{
    return _relativeTorques;
}

std::list<float> Rigidbody::GetRelativeTorquesTime()
{
    return _relativeTorquesTime;
}