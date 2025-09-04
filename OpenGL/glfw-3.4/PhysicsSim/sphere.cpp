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

	for (int i = 0; i < this->stacks; i++)
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

}

void Sphere::GenSphereIndices()
{

}