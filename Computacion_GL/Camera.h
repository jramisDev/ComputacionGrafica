#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


class Camera
{
public:

	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	int width;
	int height;

	//float sensitivity;
	float speed = 0.1f;

	Camera(int InWidth, int InHeight, glm::vec3 InPosition);

	void CameraMatrix(float InFOV, float InNearPlane, float InFarPlane, unsigned int InShaderID, const char* InUniform);

	void CameraInputs(GLFWwindow* InWindow);
};

