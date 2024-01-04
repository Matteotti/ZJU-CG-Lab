#pragma once

#include <vector>

#include "Systems/LogSystem.h"
#include "glm/glm.hpp"

namespace ToolFunctions
{
glm::vec3 NormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

glm::vec4 PlaneEquation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

glm::vec3 LinePlaneIntersection(glm::vec3 linePoint, glm::vec3 lineDirection, glm::vec4 planeEquation);

float GetProjectionLength(glm::vec3 axis, std::vector<glm::vec3> vertices);

std::vector<glm::vec3> GetSphereProjectionVertices(glm::vec3 center, float radius, glm::vec3 direction);
} // namespace ToolFunctions