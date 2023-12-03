#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "Others/Layer.h"

class Rigidbody
{
private:
    float _mass;

    glm::vec3 _angularVelocity;
    glm::vec3 _velocity;

    float _angularDrag;
    float _drag;

    float _maxAngularVelocity;
    float _maxVelocity;

    bool _useGravity;
    glm::vec3 _gravity;

    // current object's layer
    Layer _layer;
    // layer mask for collision detection
    LayerMask _layerMask;

    // process in Update in PhysicsSystem, readOnly
    std::list<glm::vec3> _forces;
    std::list<glm::vec3> _forcesPosition;
    std::list<float> _forcesTime;

    std::list<glm::vec3> _relativeForces;
    std::list<glm::vec3> _relativeForcesPosition;
    std::list<float> _relativeForcesTime;

    std::list<glm::vec3> _torques;
    std::list<float> _torquesTime;

    std::list<glm::vec3> _relativeTorques;
    std::list<float> _relativeTorquesTime;

    void OnCollisionEnter(std::shared_ptr<Rigidbody> other);
    void OnCollisionExit(std::shared_ptr<Rigidbody> other);
    void OnCollisionStay(std::shared_ptr<Rigidbody> other);

public:
    Rigidbody();

    void AddForce(glm::vec3 force, float time = 0.05f);
    void AddForceAtPosition(glm::vec3 force, glm::vec3 position, float time = 0.05f);
    void AddTorque(glm::vec3 torque, float time = 0.05f);
    void AddRelativeForce(glm::vec3 force, glm::vec3 position, float time = 0.05f);
    void AddRelativeTorque(glm::vec3 force, float time = 0.05f);

    void SetMass(float mass);
    void SetAngularVelocity(glm::vec3 angularVelocity);
    void SetVelocity(glm::vec3 velocity);
    void SetAngularDrag(float angularDrag);
    void SetDrag(float drag);
    void SetMaxAngularVelocity(float maxAngularVelocity);
    void SetMaxVelocity(float maxVelocity);
    void SetIsUseGravity(bool useGravity);
    void SetGravity(glm::vec3 gravity);
    void SetLayer(Layer layer);
    void SetLayerMask(LayerMask layerMask);

    float GetMass();
    glm::vec3 GetAngularVelocity();
    glm::vec3 GetVelocity();
    float GetAngularDrag();
    float GetDrag();
    float GetMaxAngularVelocity();
    float GetMaxVelocity();
    bool IsUseGravity();
    glm::vec3 GetGravity();
    Layer GetLayer();
    LayerMask GetLayerMask();

    std::list<glm::vec3> GetForces();
    std::list<glm::vec3> GetForcesPosition();
    std::list<float> GetForcesTime();

    std::list<glm::vec3> GetRelativeForces();
    std::list<glm::vec3> GetRelativeForcesPosition();
    std::list<float> GetRelativeForcesTime();

    std::list<glm::vec3> GetTorques();
    std::list<float> GetTorquesTime();

    std::list<glm::vec3> GetRelativeTorques();
    std::list<float> GetRelativeTorquesTime();
};
