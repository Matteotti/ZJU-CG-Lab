#pragma once

class Mesh
{
public:
    void SetVertices(unsigned VAO, unsigned VBO, unsigned vertexNum);

    void Render();

private:
    unsigned _vertexNum;
    unsigned _VAO;
    unsigned _VBO;
};
