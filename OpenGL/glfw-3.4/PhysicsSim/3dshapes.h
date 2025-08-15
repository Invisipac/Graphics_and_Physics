#pragma once

#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

class ThreeDShape {
public:
	std::vector<glm::vec3> vertices;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;

	void SetVertices();

	void UpdateVertexBuffer();

	void DrawShape();

};