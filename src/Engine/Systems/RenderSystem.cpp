#include "RenderSystem.h"

#include "Components/Mesh.h"
#include "Components/Shader.h"
#include "Components/Texture.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include "EngineSettings.h"
#include "Systems/LogSystem.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void RenderSystem::Init(bool editorMode)
{
    Super::Init(editorMode);

    gCoordinator.AddSystemSignature<RenderSystem, Mesh>();
    gCoordinator.AddSystemSignature<RenderSystem, Shader>();
    gCoordinator.AddSystemSignature<RenderSystem, Texture>();
    gCoordinator.AddSystemSignature<RenderSystem, Transform>();

    InitOpenGL();
    InitFrameBuffer();
}

void RenderSystem::Shutdown()
{
}

void RenderSystem::Update(float dt)
{

    for (auto entity : _entities)
    {
        auto &shader = gCoordinator.GetComponent<Shader>(entity);
        auto &texture = gCoordinator.GetComponent<Texture>(entity);
        auto &mesh = gCoordinator.GetComponent<Mesh>(entity);
        auto &transform = gCoordinator.GetComponent<Transform>(entity);

        shader.Activate();

        // set view & projection according to camera

        auto model = transform.GetModelMatrix();
        auto projection = *_currentSceneCamera->GetProjectionMatrix();
        auto view = *_currentSceneCamera->GetViewMatrix();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat1("ambient", 0.2f);
        shader.setVec3("lightPos", glm::vec3{20.0f, 20.0f, 20.0f});
        shader.setMat4("model", model);
        shader.setMat4("normalMatrix", glm::transpose(glm::inverse(model)));

        texture.Activate();
        mesh.Render();
    }
}

int RenderSystem::GetPriority() const
{
    return ENGINE_PRIORITY_RENDER_SYSTEM;
}

void RenderSystem::SetCurrentCamera(std::shared_ptr<Camera> camera)
{
    _currentSceneCamera = std::move(camera);
}

void RenderSystem::InitOpenGL()
{
    gladLoadGL();
    glViewport(0, 0, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);  // 默认启用深度测试
    glEnable(GL_MULTISAMPLE); // 默认启用 MSAA 以获得更好的显示效果
}

void RenderSystem::InitFrameBuffer()
{
    // codes below are copied from LearnOpenGL :)

    glGenFramebuffers(1, &_postProcFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _postProcFramebuffer);

    // create a color attachment texture
    glGenTextures(1, &_postProcFramebufferTexture);
    glBindTexture(GL_TEXTURE_2D, _postProcFramebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _postProcFramebufferTexture, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT);

    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually
    // complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer is not complete!");
    }
}

void RenderSystem::BeginFrame()
{
#if 0
    glBindFramebuffer(GL_FRAMEBUFFER, _altFramebuffer);
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::Render()
{
    ;
}

void RenderSystem::EndFrame()
{
    ;
}

GLuint RenderSystem::GetPostProcFramebufferTexture()
{
    return _postProcFramebufferTexture;
}
