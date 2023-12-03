#include "RenderSystem.h"

#include "EngineSettings.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void RenderSystem::Init()
{
    InitOpenGL();
    InitFrameBuffer();
    InitCubeVertices();
}

void RenderSystem::Update(float dt)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _altFramebuffer);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // ONLY FOR TEST:
    glPushMatrix();
    glRotatef(360.0f * sinf(0.5 * (float)(glfwGetTime())), 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0.5, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0.5, -0.5, 0);

    glColor3f(0, 0, 1);
    glVertex3f(-0.5, -0.5, 0);

    glEnd();
    glPopMatrix();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int RenderSystem::GetPriority() const
{
    return ENGINE_PRIORITY_RENDER_SYSTEM;
}

void RenderSystem::InitOpenGL()
{
    gladLoadGL();
    glViewport(0, 0, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);  // 默认启用深度测试
    glEnable(GL_MULTISAMPLE); // 默认启用 MSAA 以获得更好的显示效果
}

void RenderSystem::InitFrameBuffer()
{
    // codes below are copied from LearnOpenGL :)

    glGenFramebuffers(1, &_altFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _altFramebuffer);

    // create a color attachment texture
    glGenTextures(1, &_altFramebufferTexture);
    glBindTexture(GL_TEXTURE_2D, _altFramebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _altFramebufferTexture, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE_WINDOW_WIDTH, ENGINE_WINDOW_HEIGHT);

    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // TODO: 引入更好的错误处理机制，而不是仅仅输出一条错误信息
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
}

void RenderSystem::InitCubeVertices()
{
#if 0
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 配置 VAO。VAO 告诉 GPU 如何去解释这些数据。
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0); // x y z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))); // r g b
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif
}

GLuint RenderSystem::GetFrameAltFramebufferTexture()
{
    return _altFramebufferTexture;
}
