#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

void ThreeDShape::UpdateVertexBuffer()
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), &this->vertices[0]);
}

void ThreeDShape::CreateBuffer(unsigned int* vboNum)
{
	glGenBuffers(1, vboNum);
	glBindBuffer(GL_ARRAY_BUFFER, *vboNum);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
}

void ThreeDShape::CreateIndexBuffer(unsigned int* eboNum)
{

	glGenBuffers(1, eboNum);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboNum);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);
}

void ThreeDShape::CreateAllBuffers(unsigned int* vboNum, unsigned int* eboNum, unsigned int* vaoNum)
{
	glGenVertexArrays(1, vaoNum);
	glBindVertexArray(*vaoNum);

	this->CreateBuffer(vboNum);
	this->CreateIndexBuffer(eboNum);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void ThreeDShape::MoveShape(int updateRate, int &currentFrame)
{
	/*currentFrame += 1;

	if (currentFrame >= updateRate)
	{*/
	this->shapePosition += this->velocity;
	this->velocity += this->acceleration;	
	//std::cout << this->shapePosition.x << std::endl;
	/*currentFrame = 0;*/
	//}
}

void ThreeDShape::Collide(char direction, float energyLoss)
{
	if (direction == 'x')
	{
		this->velocity.x = - energyLoss * this->velocity.x;
	}
	else if (direction == 'y')
	{
		this->velocity.y = -energyLoss * this->velocity.y;
	}
	else if (direction == 'z')
	{
		this->velocity.z = -energyLoss * this->velocity.z;
	}
	
}

void ThreeDShape::CheckCollision(float rad, glm::vec3 bounds)
{
	if (this->shapePosition.x + rad > bounds.x)
	{
		//std::cout << "a" << std::endl;
		this->Collide('x', 0.95);
		this->shapePosition.x = bounds.x - rad;
	}

	if (this->shapePosition.x - rad < -bounds.x)
	{
		this->Collide('x', 0.95);
		this->shapePosition.x = -bounds.x + rad;
	}

	if (this->shapePosition.y + rad > bounds.y)
	{
		//std::cout << "a" << std::endl;
		this->Collide('y', 0.95);
		this->shapePosition.y = bounds.y - rad;
	}

	if (this->shapePosition.y - rad < -bounds.y)
	{
		this->Collide('y', 0.95);
		this->shapePosition.y = -bounds.y + rad;
	}


	if (this->shapePosition.z + rad > bounds.z)
	{
		//std::cout << "a" << std::endl;
		this->Collide('z', 0.95);
		this->shapePosition.z = bounds.z - rad;
	}

	if (this->shapePosition.z - rad < -bounds.z)
	{
		this->Collide('z', 0.95);
		this->shapePosition.z = -bounds.z + rad;
	}
}

void ThreeDShape::DrawShape(unsigned int vaoNum)
{
	glBindVertexArray(vaoNum);

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}