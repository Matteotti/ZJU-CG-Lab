#pragma once

#include "System.h"

#include <glad/glad.h>

class RenderSystem : public System
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    void InitOpenGL();
    void InitFrameBuffer();

    void BeginFrame();
    void Render();
    void EndFrame();

    GLuint GetPostProcFramebufferTexture();

private:
    GLuint _postProcFramebuffer, _postProcFramebufferTexture;
};
