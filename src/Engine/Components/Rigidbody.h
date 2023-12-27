#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "Others/Layer.h"
#include "Systems/LogSystem.h"

class Rigidbody
{
private:
    float _mass;
    float _inertiaTensor;

    glm::vec3 _angularVelocity;
    glm::vec3 _velocity;

    float _angularDrag;
    float _drag;

    float _maxAngularVelocity;
    float _maxVelocity;

    bool _useGravity;
    glm::vec3 _gravity;

    // process in Update in PhysicsSystem, readOnly
    std::vector<glm::vec3> _forces;
    std::vector<glm::vec3> _forcesPosition;
    std::vector<float> _forcesTime;

    std::vector<glm::vec3> _relativeForces;
    std::vector<glm::vec3> _relativeForcesPosition;
    std::vector<float> _relativeForcesTime;

    // TODO: Do we really need these?
    void OnCollisionEnter(std::shared_ptr<Rigidbody> other);
    void OnCollisionExit(std::shared_ptr<Rigidbody> other);
    void OnCollisionStay(std::shared_ptr<Rigidbody> other);

public:
    Rigidbody();

    void AddForce(glm::vec3 force, float time = 0.05f);
    void AddForceAtPosition(glm::vec3 force, glm::vec3 position, float time = 0.05f);
    void AddRelativeForce(glm::vec3 force, glm::vec3 position, float time = 0.05f);

    void SetMass(float mass);
    void SetInertiaTensor(float inertiaTensor);
    void SetAngularVelocity(glm::vec3 angularVelocity);
    void SetVelocity(glm::vec3 velocity);
    void SetAngularDrag(float angularDrag);
    void SetDrag(float drag);
    void SetMaxAngularVelocity(float maxAngularVelocity);
    void SetMaxVelocity(float maxVelocity);
    void SetIsUseGravity(bool useGravity);
    void SetGravity(glm::vec3 gravity);

    float GetMass() const;
    float GetInertiaTensor() const;
    glm::vec3 GetAngularVelocity() const;
    glm::vec3 GetVelocity() const;
    float GetAngularDrag() const;
    float GetDrag() const;
    float GetMaxAngularVelocity() const;
    float GetMaxVelocity() const;
    bool IsUseGravity() const;
    glm::vec3 GetGravity() const;

    std::vector<glm::vec3> &GetForces();
    std::vector<glm::vec3> &GetForcesPosition();
    std::vector<float> &GetForcesTime();

    std::vector<glm::vec3> &GetRelativeForces();
    std::vector<glm::vec3> &GetRelativeForcesPosition();
    std::vector<float> &GetRelativeForcesTime();
};
