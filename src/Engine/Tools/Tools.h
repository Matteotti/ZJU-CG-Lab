#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace ToolFunctions
{
    glm::vec3 NormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

    float GetProjectionLength(glm::vec3 axis, std::vector<glm::vec3> vertices);

    std::vector<glm::vec3> GetSphereProjectionVertices(glm::vec3 center, float radius, glm::vec3 direction);
}