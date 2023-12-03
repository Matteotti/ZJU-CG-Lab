#pragma once

#include "System.h"

#include <glad/glad.h>

class RenderSystem : public System
{
public:
    virtual void Init() override;
    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void InitOpenGL();
    void InitFrameBuffer();
    void InitCubeVertices();

    GLuint GetFrameAltFramebufferTexture();

private:
    GLuint _altFramebuffer, _altFramebufferTexture;
    GLuint _cubeVAO, _cubeVBO;
};
