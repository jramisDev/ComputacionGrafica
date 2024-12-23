#include "Camera.h"

Camera::Camera(int InWidth, int InHeight, glm::vec3 InPosition)
{
	Camera::width = InWidth;
	Camera::height = InHeight;
	Camera::Position = InPosition;
}

void Camera::CameraMatrix(float InFOV, float InNearPlane, float InFarPlane, unsigned int InShaderID, const char* InUniform)
{

}

void Camera::CameraInputs(GLFWwindow* InWindow)
{
}
