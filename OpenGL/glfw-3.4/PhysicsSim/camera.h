#pragma once
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>


class Camera {
	glm::vec3 position;
public:
	Camera(glm::vec3 startPosition);

	void MoveCamera();

	void AutoMoveCamera();

	glm::mat4 GetCameraMatrix(glm::vec3 target);
};