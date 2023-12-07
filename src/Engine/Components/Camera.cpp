#include "Camera.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#ifndef ORTHO
#define ORTHO true
#endif
#ifndef PERSPECTIVE
#define PERSPECTIVE false
#endif

std::shared_ptr<glm::mat4> Camera::GetViewMatrix()
{
    return _viewMatrix;
}

std::shared_ptr<glm::mat4> Camera::GetProjectionMatrix()
{
    return _projectionMatrix;
}

void Camera::SetViewMatrix(std::shared_ptr<glm::mat4> viewMatrix)
{
    _viewMatrix = std::move(viewMatrix);
}

void Camera::SetProjectionMatrix(std::shared_ptr<glm::mat4> projectionMatrix)
{
    _projectionMatrix = std::move(projectionMatrix);
}

void Camera::Translate(float x, float y, float z)
{
    *_viewMatrix = glm::translate(*_viewMatrix, glm::vec3(x, y, z));
}

void Camera::Rotate(float pitch, float yaw, float roll)
{
    *_viewMatrix = glm::rotate(*_viewMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    *_viewMatrix = glm::rotate(*_viewMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    *_viewMatrix = glm::rotate(*_viewMatrix, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::SetProjectionMode(bool isOrtho)
{
    _isOrtho = isOrtho;
}

void Camera::LookAt(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, float upX,
                    float upY, float upZ)
{
    *_viewMatrix =
        glm::lookAt(glm::vec3(posX, posY, posZ), glm::vec3(targetX, targetY, targetZ), glm::vec3(upX, upY, upZ));
}

void Camera::Perspective(float fov, float aspectRatio, float near, float far)
{
    assert(!_isOrtho || "Cannot set perspective projection on orthographic camera");
    *_projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
}

void Camera::Frustum(float left, float right, float bottom, float top, float near, float far)
{
    assert(!_isOrtho || "Cannot set frustum projection on orthographic camera");
    *_projectionMatrix = glm::frustum(left, right, bottom, top, near, far);
}
