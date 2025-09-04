#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

void ThreeDShape::UpdateVertexBuffer()
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), &this->vertices[0]);
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

void ThreeDShape::CheckCollision()
{
	if (this->shapePosition.x + 0.5 > 2)
	{
		//std::cout << "a" << std::endl;
		this->velocity.x = -this->velocity.x;
		this->shapePosition.x = 1.5;
	}

	if (this->shapePosition.x - 0.5 < -2)
	{
		this->velocity.x = -this->velocity.x;
		this->shapePosition.x = -1.5;
	}

	if (this->shapePosition.y + 0.5 > 2)
	{
		//std::cout << "a" << std::endl;
		this->velocity.y = -this->velocity.y;
		this->shapePosition.y = 1.5;
	}

	if (this->shapePosition.y - 0.5 < -2)
	{
		this->velocity.y = -this->velocity.y;
		this->shapePosition.y = -1.5;
	}


	if (this->shapePosition.z + 0.5 > 2)
	{
		//std::cout << "a" << std::endl;
		this->velocity.z = -this->velocity.z;
		this->shapePosition.z = 1.5;
	}

	if (this->shapePosition.z - 0.5 < -2)
	{
		this->velocity.z = -this->velocity.z;
		this->shapePosition.z = -1.5;
	}
}