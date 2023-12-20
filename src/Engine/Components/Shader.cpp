#include "Shader.h"

#include <glad/glad.h>

unsigned Shader::GetProgram()
{
    return _program;
}

void Shader::SetProgram(unsigned program)
{
    _program = program;
}

void Shader::Activate()
{
    glUseProgram(_program);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec)
{
    glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, &vec[0]);
}

void Shader::setInt1(const std::string &name, int val)
{
    glUniform1i(glGetUniformLocation(_program, name.c_str()), val);
}

void Shader::setFloat1(const std::string &name, float val)
{
    glUniform1f(glGetUniformLocation(_program, name.c_str()), val);
}
