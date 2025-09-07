//#include "3dshapes.h"
#define _USE_MATH_DEFINES
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "sphere.h"
#include <math.h>

void Sphere::GenSphereVertices(float r)
{
	float theta = glm::radians(M_PI / this->stacks);
	float phi = glm::radians((2 * M_PI) / this->sectors);

	Vertex topVertex = Vertex{glm::vec3(0, r, 0), this->RED, glm::vec3(0, 1, 0) };
	Vertex bottomVertex = Vertex{ glm::vec3(0, -r, 0), this->RED, glm::vec3(0, 1, 0) };

	this->vertices.push_back(topVertex);

	for (int i = 1; i < this->stacks; i++)
	{
		for (int j = 0; j < this->sectors; j++)
		{
			float y = r * sin(i * theta);
			float x = r * cos(i * theta) * cos(j * phi);
			float z = r * cos(i * theta) * sin(j * phi);

			glm::vec3 coord = glm::vec3(x, y, z);

			glm::vec3 colour = this->RED;

			glm::vec3 normal = (1 / r) * coord;

			Vertex vert = Vertex{ coord, colour, normal };

			this->vertices.push_back(vert);
		}
	}

	this->vertices.push_back(bottomVertex);

}

void Sphere::GenSphereIndices()
{
	//top surface
	for (int i = 0; i < this->sectors - 1; i++)
	{
		this->indices.push_back(0);
		this->indices.push_back(i + 1);
		this->indices.push_back(i + 2);
	}

	//middle of sphere
	for (int i = 1; i < this->stacks; i++)
	{
		for (int j = 0; j < this->sectors; j++)
		{

		}
	}
}