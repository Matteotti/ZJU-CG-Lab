#include "Systems/PhysicsSystem.h"

#include "Components/Rigidbody.h"
#include "Components/Transform.h"
#include "Coordinator.h"

#include "EngineSettings.h"

void PhysicsSystem::Init(bool editorMode)
{
    Super::Init(editorMode);

    gCoordinator.AddSystemSignature<PhysicsSystem, Rigidbody>();
}

void PhysicsSystem::Shutdown()
{
}

void PhysicsSystem::Update(float dt)
{
    // What should you do in physics system?
    for (auto entity : _entities)
    {
        Transform &transform = gCoordinator.GetComponent<Transform>(entity);
        Rigidbody &rigidbody = gCoordinator.GetComponent<Rigidbody>(entity);
        // 1 Apply gravity
        rigidbody.AddRelativeForce(rigidbody.GetGravity() * rigidbody.GetMass(), glm::vec3(0.0f), dt);
        // 2. Update all rigidbodies' velocity & angular velocity according to the forces in the lists
        // 2.1. Update common forces
        std::vector<glm::vec3> &commanForces = rigidbody.GetForces();
        std::vector<glm::vec3> &commonForcesPos = rigidbody.GetForcesPosition();
        std::vector<float> &commonForcesTime = rigidbody.GetForcesTime();
        int commonForceNums = commanForces.size();
        for (int i = 0; i < commonForceNums; i++)
        {
            glm::vec3 force = commanForces[i];
            glm::vec3 position = commonForcesPos[i];

            LOG_INFO("Resolve common force %d {%f, %f, %f}, world pos {%f, %f, %f}, last time %f", i, force.x, force.y,
                     force.z, position.x, position.y, position.z, commonForcesTime[i]);

            glm::vec3 torque = glm::cross(position - transform.GetPosition(), force);

            glm::vec3 accelaration = force / rigidbody.GetMass();
            glm::vec3 angularAccelaration = torque / rigidbody.GetInertiaTensor();

            rigidbody.SetVelocity(rigidbody.GetVelocity() + accelaration * dt);
            rigidbody.SetAngularVelocity(rigidbody.GetAngularVelocity() + glm::degrees(angularAccelaration) * dt);

            commonForcesTime[i] -= dt;

            if (commonForcesTime[i] <= 0)
            {
                commanForces.erase(commanForces.begin() + i);
                commonForcesPos.erase(commonForcesPos.begin() + i);
                commonForcesTime.erase(commonForcesTime.begin() + i);
                i--;
                commonForceNums--;
            }
        }

        // 2.2. Update relative forces
        std::vector<glm::vec3> &relativeForces = rigidbody.GetRelativeForces();
        std::vector<glm::vec3> &relativeForcesPos = rigidbody.GetRelativeForcesPosition();
        std::vector<float> &relativeForcesTime = rigidbody.GetRelativeForcesTime();
        int relativeForceNums = relativeForces.size();
        for (int i = 0; i < relativeForceNums; i++)
        {
            glm::vec3 force = relativeForces[i];
            glm::vec3 position = relativeForcesPos[i];

            if (relativeForces[i] != rigidbody.GetGravity() * rigidbody.GetMass())
                LOG_INFO("Resolve relative force %d {%f, %f, %f}, local pos {%f, %f, %f}, last time %f", i, force.x,
                         force.y, force.z, position.x, position.y, position.z, relativeForcesTime[i]);

            glm::vec3 torque = glm::cross(position, force);

            glm::vec3 accelaration = force / rigidbody.GetMass();
            glm::vec3 angularAccelaration = torque / rigidbody.GetInertiaTensor();

            rigidbody.SetVelocity(rigidbody.GetVelocity() + accelaration * dt);
            rigidbody.SetAngularVelocity(rigidbody.GetAngularVelocity() + glm::degrees(angularAccelaration) * dt);

            relativeForcesTime[i] -= dt;

            if (relativeForcesTime[i] <= 0)
            {
                relativeForces.erase(relativeForces.begin() + i);
                relativeForcesPos.erase(relativeForcesPos.begin() + i);
                relativeForcesTime.erase(relativeForcesTime.begin() + i);

                i--;
                relativeForceNums--;
            }
        }

        // 2.3. Apply impulse
        std::vector<glm::vec3> &impulses = rigidbody.GetImpulse();
        std::vector<glm::vec3> &impulsePos = rigidbody.GetImpulsePosition();
        for (int i = 0; i < impulses.size(); i++)
        {
            glm::vec3 impulse = impulses[i];
            glm::vec3 position = impulsePos[i];

            glm::vec3 torque = glm::cross(position, impulse);

            glm::vec3 accelaration = impulse / rigidbody.GetMass();
            glm::vec3 angularAccelaration = torque / rigidbody.GetInertiaTensor();

            rigidbody.SetVelocity(rigidbody.GetVelocity() + accelaration);
            rigidbody.SetAngularVelocity(rigidbody.GetAngularVelocity() + glm::degrees(angularAccelaration));

            LOG_INFO("Impulse made acceleration {%f, %f, %f}, angular acceleration {%f, %f, %f}.", accelaration.x,
                     accelaration.y, accelaration.z, angularAccelaration.x, angularAccelaration.y,
                     angularAccelaration.z);

            impulses.erase(impulses.begin() + i);
            impulsePos.erase(impulsePos.begin() + i);
            i--;
        }
    }
}

int PhysicsSystem::GetPriority() const
{
    return ENGINE_PRIORITY_PHYSICS_SYSTEM;
}