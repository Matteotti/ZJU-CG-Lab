#include "Mesh.h"

#include <glad/glad.h>

void Mesh::SetVertices(unsigned VAO, unsigned VBO, unsigned vertexNum)
{
    _VAO = VAO;
    _VBO = VBO;
    _vertexNum = vertexNum;
}

void Mesh::Render()
{
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _vertexNum);
}
