#include "Tools.h"

#include <algorithm>

namespace ToolFunctions
{
glm::vec3 NormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    glm::vec3 v1v2 = v2 - v1;
    glm::vec3 v1v3 = v3 - v1;

    return glm::normalize(glm::cross(v1v2, v1v3));
}

glm::vec4 PlaneEquation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    glm::vec3 normal = NormalVector(v1, v2, v3);

    float depth = -glm::dot(normal, v1);

    return glm::vec4(normal, depth);
}

// WARNING: 请确保 lineDirection 与 planeEquation 的法向量不平行
// FIXME 有问题
glm::vec3 LinePlaneIntersection(glm::vec3 linePoint, glm::vec3 lineDirection, glm::vec4 planeEquation)
{
    if (glm::dot(lineDirection, glm::vec3(planeEquation)) == 0)
    {
        LOG_WARNING("LinePlaneIntersection: lineDirection and planeEquation are parallel.");
    }

    float t =
        -glm::dot(planeEquation, glm::vec4(linePoint, 1.0f)) / glm::dot(planeEquation, glm::vec4(lineDirection, 0.0f));

    return linePoint + t * lineDirection;
}

float GetProjectionLength(glm::vec3 axis, std::vector<glm::vec3> vertices)
{
    std::vector<float> projectionLength;

    for (int i = 0; i < vertices.size(); i++)
    {
        projectionLength.push_back(glm::dot(axis, vertices[i]));
    }

    std::sort(projectionLength.begin(), projectionLength.end(), [](float a, float b) { return a < b; });

    return projectionLength[projectionLength.size() - 1] - projectionLength[0];
}

std::vector<glm::vec3> GetSphereProjectionVertices(glm::vec3 center, float radius, glm::vec3 direction)
{
    std::vector<glm::vec3> vertices;

    glm::vec3 v1 = center + radius * glm::normalize(direction);
    glm::vec3 v2 = center - radius * glm::normalize(direction);

    vertices.push_back(v1);
    vertices.push_back(v2);

    return vertices;
}
} // namespace ToolFunctions