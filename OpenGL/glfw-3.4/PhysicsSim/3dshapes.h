#pragma once

#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 colour;
};

class ThreeDShape {
	std::vector<Vertex> vertices;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;

	void SetVertices();

	void UpdateVertexBuffer();

	void DrawShape();

};