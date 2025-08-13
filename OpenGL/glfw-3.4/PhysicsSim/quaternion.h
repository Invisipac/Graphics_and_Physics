#pragma once

#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>


class Quaternion {

	glm::vec4 Q;

public:

	Quaternion(glm::vec4);

	Quaternion(glm::vec3 axis, float angle);

	//void PolarToVec(glm::vec3 axis, float angle);

	glm::mat4 GetRotationMatrix();

	glm::mat4 Compose(glm::mat4 leftMat, glm::mat4 rightMat);

	Quaternion Multiply(Quaternion q1, Quaternion q2);

};