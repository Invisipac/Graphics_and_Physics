#include "camera.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 startPosition)
{
	this->position = startPosition;
}

glm::mat4 Camera::GetCameraMatrix(glm::vec3 target)
{
	glm::mat4 view = glm::lookAt(this->position, target, glm::vec3(0.0f, 1.0f, 0.0f));

	return view;
}

void Camera::AutoMoveCamera()
{
	float time = glfwGetTime();

	this->position = glm::vec3(cos(time)*10, 0.0f, sin(time)*10);
}
