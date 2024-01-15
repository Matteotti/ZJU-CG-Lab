#pragma once

#include "System.h"

#include <Components/Camera.h>
#include <glad/glad.h>

class RenderSystem : public System
{
public:
    virtual void Init(bool editorMode) override;
    virtual void Shutdown() override;

    virtual void Update(float dt) override;
    virtual int GetPriority() const override;

    std::shared_ptr<Camera> GetCurrentCamera();
    void SetCurrentCamera(std::shared_ptr<Camera> camera);

    void InitOpenGL();
    void InitFrameBuffer();

    void BeginFrame();
    void Render();
    void EndFrame();

    GLuint GetPostProcFramebufferTexture();

private:
    GLuint _postProcFramebuffer, _postProcFramebufferTexture;
    GLuint _postProcFramebufferAux, _postProcFramebufferTextureAux;
    std::shared_ptr<Camera> _currentSceneCamera;
};
