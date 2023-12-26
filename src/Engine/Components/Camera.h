#pragma once

#include <glm/glm.hpp>
#include <memory>

class Camera : public std::enable_shared_from_this<Camera>
{
public:
    Camera();

    std::shared_ptr<glm::mat4> GetViewMatrix();

    std::shared_ptr<glm::mat4> GetProjectionMatrix();

    void SetViewMatrix(std::shared_ptr<glm::mat4> viewMatrix);

    void SetProjectionMatrix(std::shared_ptr<glm::mat4> projectionMatrix);

    void Translate(float x, float y, float z);

    void TranslateTo(float x, float y, float z);

    void TranslateTo(glm::vec3 position);

    void Rotate(float x, float y, float z); // parameters in angles

    void RotateTo(float x, float y, float z); // parameters in angles

    void RotateTo(glm::vec3 rotation); // parameters in angles

    void SetProjectionMode(bool isOrtho); // can use ORTHO and PERSPECTIVE macros

    void LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    void Perspective(float fov, float aspectRatio, float near, float far);

    void Ortho(float left, float right, float bottom, float top, float near, float far);

    void Frustum(float left, float right, float bottom, float top, float near, float far);

    void SetAsCurrentSceneCamera();

    void SetAsCurrentGameplayCamera();

private:
    void UpdateViewMatrix();

    glm::vec3 _position;
    glm::vec3 _target;
    glm::vec3 _up;
    std::shared_ptr<glm::mat4> _viewMatrix;
    std::shared_ptr<glm::mat4> _projectionMatrix;
    bool _isOrtho = true; // true for ortho, false for perspective
};