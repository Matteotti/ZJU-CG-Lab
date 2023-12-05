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

    float GetProjectionLength(glm::vec3 axis, std::vector<glm::vec3> vertices)
    {
        std::vector<float> projectionLength;

        for (int i = 0; i < vertices.size(); i++)
        {
            projectionLength.push_back(glm::dot(axis, vertices[i]));
        }

        std::sort(projectionLength.begin(), projectionLength.end(), [](float a, float b)
                  { return a < b; });

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
}