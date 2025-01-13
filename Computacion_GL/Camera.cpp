#include "Camera.h"

Camera::Camera(int InWidth, int InHeight, glm::vec3 InPosition)
{
	Camera::width = InWidth;
	Camera::height = InHeight;
	Camera::Position = InPosition;
}

void Camera::CameraMatrix(float InFOV, float InNearPlane, float InFarPlane, unsigned int InShaderID, const char* InUniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = lookAt(Position, Position + Orientation, Up);

	projection = glm::perspective(InFOV, (float)(width / height), InNearPlane, InFarPlane);

	GLuint CameraUniform = glGetUniformLocation(InShaderID, InUniform);
	glUniformMatrix4fv(CameraUniform, 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::CameraInputs(GLFWwindow* InWindow)
{
	if (glfwGetKey(InWindow, GLFW_KEY_W)) {
		Position += speed * Orientation;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_A)) {
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(InWindow, GLFW_KEY_S)) {
		Position += speed * -Orientation;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_D)) {
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}

	if (glfwGetKey(InWindow, GLFW_KEY_Q)) {
		Position += speed * Up;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_E)) {
		Position += speed * -Up;
	}
	
	if (glfwGetKey(InWindow, GLFW_KEY_LEFT_CONTROL)) {
		speed = 0.1f;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_LEFT_SHIFT)) {
		speed = 1.0f;
	}

	if (glfwGetMouseButton(InWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (!rightButtonHold) {
			rightButtonHold = true;
			glfwGetCursorPos(InWindow, &lastPosMouseX, &lastPosMouseY);
		} else {
			double currentPosMouseX, currentPosMouseY;
			glfwGetCursorPos(InWindow, &currentPosMouseX, &currentPosMouseY);

			double deltaX = currentPosMouseX - lastPosMouseX;
			double deltaY = currentPosMouseY - lastPosMouseY;

			Orientation = glm::rotate(Orientation, -glm::radians((float)deltaX * mouseSensitivity), Up);
			glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
			Orientation = glm::rotate(Orientation, -glm::radians((float)deltaY * mouseSensitivity), right);

			lastPosMouseX = currentPosMouseX;
			lastPosMouseY = currentPosMouseY;
		}
	} else {
		rightButtonHold = false;
	}
}
