#define _CRT_SECURE_NO_WARNINGS
// 以前写的一个 demo，把不必要的地方删掉了
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>

#include "Shader.h"

#define WINDOW_TITLE "TEST"

GLFWwindow *window;

GLuint VAO, VBO;

void initVertices();

int main()
{
    // Init OpenGL Libraries
    glfwInit();
    // 指定 OpenGL 版本为 4.6 (latest version)
    // 这个 “指定” 并不是 requirement。如果显卡不支持此版本，就会自动提供更低版本的 profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    // 这里有必要解释一下这个 COMPAT PROFILE，它是 compatibility profile 的缩写，
    // 还有另外一种 profile 叫 core profile。
    // 前者相当于包含 legacy opengl 和 modern opengl，后者只有 modern opengl 的部分。
    // 具体可参考：https://blog.csdn.net/hankern/article/details/90340797

    glfwWindowHint(GLFW_SAMPLES, 4); // enable MSAA
    window = glfwCreateWindow(1024, 1024, WINDOW_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    gladLoadGL();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 1024, 1024);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    initVertices();
    Shader shader("triangle.vshader", "triangle.fshader");

    // Start main loop
    std::chrono::time_point<std::chrono::steady_clock> prev, now;
    float delta_time, delta_time_avg = 0.016f, sec_cnt = 0.0f;
    char title_buf[64];
    while (!glfwWindowShouldClose(window))
    {
        // Calculate FPS
        now = std::chrono::steady_clock::now();
        delta_time = std::chrono::duration_cast<std::chrono::microseconds>(now - prev).count() / 1000000.0f;
        delta_time_avg = delta_time_avg * 0.9f + delta_time * 0.1f;
        prev = now;
        sec_cnt += delta_time;
        if (sec_cnt >= 1.0f)
        {
            sec_cnt = 0.0f;
            std::sprintf(title_buf, WINDOW_TITLE " [FPS: %.0f]", 1.0f / delta_time_avg);
            glfwSetWindowTitle(window, title_buf);
        }

        // Clear back buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // do rendering here
#if 0
        // 为了方便，这里用的还是 legacy opengl
        glPushMatrix();
        glRotatef(360.0f * sinf(0.1 * (float)(glfwGetTime())), 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);

        glColor3f(1, 0, 0);
        glVertex3f(0, 0.5, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0.5, -0.5, 0);

        glColor3f(0, 0, 1);
        glVertex3f(-0.5, -0.5, 0);

        glEnd();
        glPopMatrix();
#endif

#if 1
        // modern opengl ver.
        glm::mat4 model(1.0f); // 相当于 glLoadIdentity
        model = glm::rotate(model, glm::radians(360.0f * sinf(0.1f * (float)glfwGetTime())),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        shader.activate();
        shader.setMat4("model", model);
        glBindVertexArray(VAO); // 想要渲染某物体时，只需要绑定创建该物体时使用的 VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}

void initVertices()
{
    float vertices[] =
        {// x y z | r g b
         0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    // 配置 VBO。VBO 存储的是顶点数据，但没有任何额外的约束，你可以把它想象为一个普通的数组，只不过这个数组在显存里面
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
}
