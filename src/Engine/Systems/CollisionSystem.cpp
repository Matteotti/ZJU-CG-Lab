#include "CollisionSystem.h"

#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include "Components/Transform.h"

#include "Coordinator.h"

#include "EngineSettings.h"

void CollisionSystem::Init(bool editorMode)
{
    Super::Init(editorMode);

    gCoordinator.AddSystemSignature<CollisionSystem, Rigidbody>();
    gCoordinator.AddSystemSignature<CollisionSystem, Collider>();
    gCoordinator.AddSystemSignature<CollisionSystem, Transform>();
}

void CollisionSystem::Shutdown()
{
}

void CollisionSystem::Update(float dt)
{
    for (auto entity : _entities)
    {
        auto &rigidbody = gCoordinator.GetComponent<Rigidbody>(entity);
        auto &collider = gCoordinator.GetComponent<Collider>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        // 1. Update Collider according to rotation & scale
        rigidbody.UpdateRelativeForcesPosition(transform.GetRotation(), transform.GetScale());
        collider.SetPosition(transform.GetPosition());
        collider.SetRotation(transform.GetRotation());
        collider.SetScale(transform.GetScale());
    }

    for (auto entity : _entities)
    {
        auto &rigidbody = gCoordinator.GetComponent<Rigidbody>(entity);
        auto &collider = gCoordinator.GetComponent<Collider>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        // 2. Check for all collisions
        // TODO: Need Improvement: 8-tree......
        for (auto other : _entities)
        {
            if (other == entity)
                continue;
            auto &otherCollider = gCoordinator.GetComponent<Collider>(other);
            auto &otherRigidbody = gCoordinator.GetComponent<Rigidbody>(other);
            auto &otherTransform = gCoordinator.GetComponent<Transform>(other);
            std::shared_ptr<CollisionInfo> info = collider.CheckCollision(otherCollider);
            if (!info->_isColliding)
                continue;
            // 3. Resolve all collisions (Apply forces to rigidbodies, considering PhysicsMaterial)

            // 恢复系数
            float coefficent = collider.GetPhysicsMaterial()->CombinedBounciness(*otherCollider.GetPhysicsMaterial());
            // LOG_INFO("coefficent: %f", coefficent);
            //  穿透深度
            glm::vec3 collisionNormal = info->_collisionNormal;
            // LOG_INFO("collisionNormal: %f, %f, %f", collisionNormal.x, collisionNormal.y, collisionNormal.z);
            //  本物体碰撞点相对于本物体质心的位置
            glm::vec3 selfRelativeCollisionPoint = info->_selfCollisionPoint - transform.GetPosition();
            // LOG_INFO("selfRelativeCollisionPoint: %f, %f, %f", selfRelativeCollisionPoint.x,
            //          selfRelativeCollisionPoint.y, selfRelativeCollisionPoint.z);
            //  对方物体碰撞点相对于对方物体质心的位置
            glm::vec3 otherRelativeCollisionPoint = info->_otherCollisionPoint - otherTransform.GetPosition();
            // LOG_INFO("otherRelativeCollisionPoint: %f, %f, %f", otherRelativeCollisionPoint.x,
            // otherRelativeCollisionPoint.y, otherRelativeCollisionPoint.z);
            // 本物体质量
            float selfMass = rigidbody.GetMass();
            // LOG_INFO("selfMass: %f", selfMass);
            //  对方物体质量
            float otherMass = otherRigidbody.GetMass();
            // LOG_INFO("otherMass: %f", otherMass);
            //  本物体转动惯量
            float selfInertiaTensor = rigidbody.GetInertiaTensor();
            // LOG_INFO("selfInertiaTensor: %f", selfInertiaTensor);
            //  对方物体转动惯量
            float otherInertiaTensor = otherRigidbody.GetInertiaTensor();
            // LOG_INFO("otherInertiaTensor: %f", otherInertiaTensor);
            //  本物体速度
            glm::vec3 selfWorldSpeed =
                rigidbody.GetVelocity() + glm::cross(rigidbody.GetAngularVelocity(), selfRelativeCollisionPoint);
            // LOG_INFO("selfWorldSpeed: %f, %f, %f", selfWorldSpeed.x, selfWorldSpeed.y, selfWorldSpeed.z);
            //  对方物体速度
            glm::vec3 otherWorldSpeed = otherRigidbody.GetVelocity() +
                                        glm::cross(otherRigidbody.GetAngularVelocity(), otherRelativeCollisionPoint);
            // LOG_INFO("otherWorldSpeed: %f, %f, %f", otherWorldSpeed.x, otherWorldSpeed.y,
            // otherWorldSpeed.z);
            //  相对速度
            glm::vec3 relativeSpeed = selfWorldSpeed - otherWorldSpeed;
            // LOG_INFO("relativeSpeed: %f, %f, %f", relativeSpeed.x, relativeSpeed.y, relativeSpeed.z);

            // TODO: considering friction
            // 公式见群里的rigid2.pdf
            glm::vec3 impulse = -(1 + coefficent) * glm::dot(collisionNormal, relativeSpeed) /
                                (1 / selfMass + 1 / otherMass +
                                 glm::dot(glm::cross(selfRelativeCollisionPoint, collisionNormal),
                                          glm::cross(selfRelativeCollisionPoint, collisionNormal)) /
                                     selfInertiaTensor +
                                 glm::dot(glm::cross(otherRelativeCollisionPoint, collisionNormal),
                                          glm::cross(otherRelativeCollisionPoint, collisionNormal)) /
                                     otherInertiaTensor) *
                                collisionNormal * 1.0f;

            // glm::vec3 impulse = -coefficent * glm::dot(collisionNormal, relativeSpeed) * collisionNormal;

            // apply impulse to self
            rigidbody.AddImpulse(impulse, selfRelativeCollisionPoint);

            // apply impulse to other
            otherRigidbody.AddImpulse(-impulse, otherRelativeCollisionPoint);
        }
    }
}

int CollisionSystem::GetPriority() const
{
    return ENGINE_PRIORITY_COLLISION_SYSTEM;
}