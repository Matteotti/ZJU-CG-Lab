#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string &vshader_path, const std::string &fshader_path)
{
    std::string vshader_str;
    std::string fshader_str;
    std::ifstream vshader_fin;
    std::ifstream fshader_fin;
    vshader_fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fshader_fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vshader_fin.open(vshader_path);
        fshader_fin.open(fshader_path);
        std::stringstream vshader_ssm, fshader_ssm;
        vshader_ssm << vshader_fin.rdbuf();
        fshader_ssm << fshader_fin.rdbuf();
        vshader_fin.close();
        fshader_fin.close();
        vshader_str = vshader_ssm.str();
        fshader_str = fshader_ssm.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "Failed to load shader source: " << e.what() << std::endl;
    }
    const char *vshader_data = vshader_str.c_str();
    const char *fshader_data = fshader_str.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vshader_data, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fshader_data, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    checkCompileErrors(m_program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::activate()
{
    glUseProgram(m_program);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1,
                       GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec)
{
    glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &vec[0]);
}

void Shader::setInt1(const std::string &name, int val)
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), val);
}

void Shader::setFloat1(const std::string &name, float val)
{
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), val);
}

void Shader::checkCompileErrors(GLuint id, const std::string &type)
{
    static GLchar infolog[1024];

    GLint success;
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, infolog);
            std::cout << "[" << type << "] ";
            std::cout << infolog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, infolog);
            std::cout << "[" << type << "] ";
            std::cout << infolog << std::endl;
        }
    }
}
