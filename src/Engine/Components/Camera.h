#include <memory>
#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;
	
	~Camera() = default;
	
	std::shared_ptr<glm::mat4> GetViewMatrix();
	
	std::shared_ptr<glm::mat4> GetProjectionMatrix();
	
	void SetViewMatrix(std::shared_ptr<glm::mat4> viewMatrix);
	
	void SetProjectionMatrix(std::shared_ptr<glm::mat4> projectionMatrix);
	
	void Translate(float x, float y, float z);
	
	void Rotate(float pitch, float yaw, float roll);//parameters in radians
	void SetProjectionMode(bool isOrtho);//can use ORTHO and PERSPECTIVE macros
	
	void LookAt(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, float upX, float upY,
	            float upZ);
	
	void Perspective(float fov, float aspectRatio, float near, float far);
	
	void Frustum(float left, float right, float bottom, float top, float near, float far);

private:
	std::shared_ptr<glm::mat4> _viewMatrix;
	std::shared_ptr<glm::mat4> _projectionMatrix;
	bool _isOrtho = true;//true for ortho, false for perspective
};