#pragma once

#include <string>

#include <glm/glm.hpp>

class Shader
{
public:
    unsigned GetProgram();
    void SetProgram(unsigned program);

    void setMat4(const std::string &name, const glm::mat4 &matrix);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setInt1(const std::string &name, int val);
    void setFloat1(const std::string &name, float val);

    void Activate();

private:
    unsigned _program;
};
