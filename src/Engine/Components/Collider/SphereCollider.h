#pragma once
#include "BoxCollider.h"
#include "Collider.h"

class SphereCollider : public Collider
{
private:
    float _radius;
    glm::vec3 _center;

public:
    SphereCollider();

    void SetRadius(float radius);
    void SetCenter(glm::vec3 center);

    float GetRadius();
    glm::vec3 GetCenter();

    virtual bool CheckCollision(std::shared_ptr<Collider> other) override;
    virtual void Update() override;
};