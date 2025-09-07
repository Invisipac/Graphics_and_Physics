#pragma once

#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

typedef struct _Vertex {
	glm::vec3 position;
	glm::vec3 colour;
	glm::vec3 normal;
} Vertex;

class ThreeDShape {
public:
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	glm::vec3 shapePosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 RED = glm::vec3(1, 0, 0);
	glm::vec3 BLUE = glm::vec3(0, 0, 1);
	glm::vec3 GREEN = glm::vec3(0, 1, 0);

	float mass;

	void UpdateVertexBuffer();

	void MoveShape(int updateRate, int &currentFrame);

	void CheckCollision();

	void Collide(float energyLoss);

	void CreateBuffer(unsigned int* vboNum);

	void CreateIndexBuffer(unsigned int* eboNum);

	void CreateAllBuffers(unsigned int* vboNum, unsigned int* eboNum, unsigned int* vaoNum);

	void DrawShape(unsigned int vaoNum);

	void Transform(glm::mat4 transformation);
};