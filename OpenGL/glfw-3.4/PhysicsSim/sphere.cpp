//#include "3dshapes.h"
#define _USE_MATH_DEFINES
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "sphere.h"
#include <math.h>
#include "3dshapes.h"

Sphere::Sphere(int sectors, int stacks)
{
	this->sectors = sectors;
	this->stacks = stacks;
}

void Sphere::GenSphereVertices(float r)
{
	float theta = M_PI / this->stacks;
	float phi = (2 * M_PI) / this->sectors;

	Vertex topVertex = Vertex{glm::vec3(0, r, 0), this->RED, glm::vec3(0, 1, 0) };
	Vertex bottomVertex = Vertex{ glm::vec3(0, -r, 0), this->RED, glm::vec3(0, -1, 0) };

	this->vertices.push_back(bottomVertex);

	for (int i = 1; i < this->stacks; i++)
	{
		for (int j = 0; j < this->sectors; j++)
		{
			float y = r * sin(i * theta - (M_PI / 2));
			float x = r * cos(i * theta - (M_PI / 2)) * cos(j * phi);
			float z = r * cos(i * theta - (M_PI / 2)) * sin(j * phi);

			glm::vec3 coord = glm::vec3(x, y, z);

			glm::vec3 colour = this->RED;

			glm::vec3 normal = (1 / r) * coord;

			Vertex vert = Vertex{ coord, colour, normal };

			this->vertices.push_back(vert);
		}
	}

	this->vertices.push_back(topVertex);

}

void Sphere::GenSphereIndices()
{
	//bottom surface
	for (int i = 0; i < this->sectors; i++)
	{
		this->indices.push_back(0);
		this->indices.push_back(i % (this->sectors) + 1);
		this->indices.push_back((i + 1) % (this->sectors) + 1);
	}

	//middle of sphere
	for (int i = 1; i < this->stacks - 1; i++)
	{
		for (int j = 0; j < this->sectors; j++)
		{
			int offset = this->sectors * (i - 1) + 1;
			//1st triangle
			this->indices.push_back(offset + j);
			this->indices.push_back(offset + j + this->sectors);
			this->indices.push_back(offset + (j + 1) % this->sectors);
			//2nd triangle
			this->indices.push_back(offset + (j + 1) % this->sectors);
			this->indices.push_back(offset + ((j + 1) % this->sectors) + this->sectors);
			this->indices.push_back(offset + j + this->sectors);
		}
	}

	//top surface
	int topOffset = 1 + this->sectors * (this->stacks - 2);
	int last = this->vertices.size() - 1;
	for (int i = 0; i < this->sectors; i++)
	{
		this->indices.push_back(last);
		this->indices.push_back(topOffset + (i % this->sectors));
		this->indices.push_back(topOffset + ((i + 1) % this->sectors));
	}
}