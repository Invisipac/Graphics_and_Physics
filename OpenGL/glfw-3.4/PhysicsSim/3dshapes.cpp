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
	/*currentFrame = 0;*/
	//}
}

void ThreeDShape::CheckCollision()
{
	if (this->shapePosition.x + 0.5 > 2 or this->shapePosition.x - 0.5 < -2)
	{
		this->velocity = -this->velocity;
	}

	if (this->shapePosition.y + 0.5 > 2 or this->shapePosition.y - 0.5 < -2)
	{
		this->velocity = -this->velocity;
	}
}