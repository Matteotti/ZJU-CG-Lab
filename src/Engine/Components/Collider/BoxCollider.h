#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
    glm::vec3 _size;
    glm::vec3 _center;

public:
    BoxCollider();

    void SetSize(glm::vec3 size);
    void SetCenter(glm::vec3 center);

    glm::vec3 GetSize();
    glm::vec3 GetCenter();
};