#include "Camera.h"
#include "Coordinator.h"
#include "Systems/RenderSystem.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#ifndef ORTHO
#define ORTHO true
#endif
#ifndef PERSPECTIVE
#define PERSPECTIVE false
#endif

Camera::Camera()
{
    _viewMatrix = std::make_shared<glm::mat4>(glm::mat4(1.0f));
    _projectionMatrix = std::make_shared<glm::mat4>(glm::mat4(1.0f));
    _isOrtho = PERSPECTIVE;
}

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
    glm::mat4 trans = glm::mat4(1.0f);
    glm::translate(trans, glm::vec3(x, y, z));
    _position = trans * glm::vec4(_position, 1.0f);
    _target = trans * glm::vec4(_target, 1.0f);
    UpdateViewMatrix();
}

void Camera::TranslateTo(float x, float y, float z)
{
    _position = glm::vec3(x, y, z);
    UpdateViewMatrix();
}

void Camera::Rotate(float x, float y, float z)
{
    glm::vec3 direction = _target - _position;
    glm::mat4 trans = glm::mat4(1.0f);
    glm::rotate(trans, x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::rotate(trans, y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::rotate(trans, z, glm::vec3(0.0f, 0.0f, 1.0f));
    direction = trans * glm::vec4(direction, 1.0f);
    _position = direction + _position;
    _up = trans * glm::vec4(_up, 1.0f);
    UpdateViewMatrix();
}

void Camera::SetProjectionMode(bool isOrtho)
{
    _isOrtho = isOrtho;
}

void Camera::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    _position = position;
    _target = target;
    _up = up;
    UpdateViewMatrix();
}

void Camera::Perspective(float fov, float aspectRatio, float near, float far)
{
    assert(!_isOrtho || "Cannot set perspective projection on orthographic camera");
    *_projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
}

void Camera::Ortho(float left, float right, float bottom, float top, float near, float far)
{
    assert(_isOrtho || "Cannot set orthographic projection on perspective camera");
    *_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::Frustum(float left, float right, float bottom, float top, float near, float far)
{
    assert(!_isOrtho || "Cannot set frustum projection on orthographic camera");
    *_projectionMatrix = glm::frustum(left, right, bottom, top, near, far);
}

void Camera::SetAsCurrentSceneCamera()
{
    auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    renderSystem->SetCurrentCamera(shared_from_this());
}

void Camera::SetAsCurrentGameplayCamera()
{
    // TODO: Do this after we finish gameplay rendering
}

void Camera::UpdateViewMatrix()
{
    *_viewMatrix = glm::lookAt(_position, _target, _up);
}