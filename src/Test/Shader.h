#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Shader // Mainly from LearnOpenGL. Thanks!
{
public:
    Shader(const std::string &vshader_path, const std::string &fshader_path);
    ~Shader();

    void activate();
    void setMat4(const std::string &name, const glm::mat4 &matrix);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setInt1(const std::string &name, int val);
    void setFloat1(const std::string &name, float val);

private:
    void checkCompileErrors(GLuint id, const std::string &type);

private:
    GLuint m_program;
};
