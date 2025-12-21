#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "cube.h"




void Cube::SetCubeVertices() {
	this->vertices =
	{
		//Front
		OldVertex{glm::vec3(-s, s, s), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)}, //front top left
		OldVertex{glm::vec3(s, s, s), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)}, //front top right
		OldVertex{ glm::vec3(-s, -s, s), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)}, //front bottom left
		OldVertex{glm::vec3(s, -s, s), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)}, //front bottom right
		//Right
		OldVertex{glm::vec3(s, s, s), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)}, //front top right
		OldVertex{glm::vec3(s, -s, s), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)}, //front bottom right
		OldVertex{glm::vec3(s, s, -s) , glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)}, //back top right
		OldVertex{glm::vec3(s, -s, -s), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)}, //back bottom right
		//Left
		OldVertex{ glm::vec3(-s, s, s), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0)}, //front top left
		OldVertex{ glm::vec3(-s, -s, s), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0)}, //front bottom left
		OldVertex{glm::vec3(-s, s, -s), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0)}, //back top left
		OldVertex{ glm::vec3(-s, -s, -s), glm::vec3(1, 1, 0), glm::vec3(-1, 0, 0) },//back bottom left
		//Top
		OldVertex{ glm::vec3(-s, s, s), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)}, //front top left
		OldVertex{ glm::vec3(s, s, s), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0) }, //front top right
		OldVertex{ glm::vec3(-s, s, -s), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0) }, //back top left
		OldVertex{ glm::vec3(s, s, -s) , glm::vec3(0, 0, 1), glm::vec3(0, 1, 0) }, //back top right
		//Bottom
		OldVertex{ glm::vec3(-s, -s, s), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0)}, //front bottom left
		OldVertex{glm::vec3(s, -s, s), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0)}, //front bottom right
		OldVertex{ glm::vec3(-s, -s, -s), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0) },//back bottom left
		OldVertex{ glm::vec3(s, -s, -s), glm::vec3(0, 1, 1), glm::vec3(0, -1, 0) }, //back bottom right
		//Back
		OldVertex{glm::vec3(-s, s, -s), glm::vec3(1, 0, 1), glm::vec3(0, 0, -1)}, //back top left
		OldVertex{glm::vec3(s, s, -s) , glm::vec3(1, 0, 1), glm::vec3(0, 0, -1)}, //back top right
		OldVertex{glm::vec3(-s, -s, -s), glm::vec3(1, 0, 1), glm::vec3(0, 0, -1)},//back bottom left
		OldVertex{glm::vec3(s, -s, -s), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)} //back bottom right	
		}; 
	}

	void Cube::GenIndices()
	{
		for (int i = 0; i <= this->vertices.size() - 4; i += 4)
		{
			this->indices.push_back(i);
			this->indices.push_back(i + 1);
			this->indices.push_back(i + 2);

			this->indices.push_back(i + 1);
			this->indices.push_back(i + 2);
			this->indices.push_back(i + 3);
		}
	}

	void Cube::CreateCubeBuffer(unsigned int* vboNum)
	{
		glGenBuffers(1, vboNum);
		glBindBuffer(GL_ARRAY_BUFFER, *vboNum);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(OldVertex), &this->vertices[0], GL_STATIC_DRAW);
	}

	void Cube::CreateIndexBuffer(unsigned int* eboNum)
	{

		glGenBuffers(1, eboNum);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboNum);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);
	}
	
	void Cube::CreateAllBuffers(unsigned int* vboNum, unsigned int* eboNum, unsigned int* vaoNum)
	{
		glGenVertexArrays(1, vaoNum);
		glBindVertexArray(*vaoNum);

		this->CreateCubeBuffer(vboNum);
		this->CreateIndexBuffer(eboNum);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OldVertex), (void *) offsetof(OldVertex, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OldVertex), (void *) offsetof(OldVertex, colour));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(OldVertex), (void *) offsetof(OldVertex, normal));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void Cube::DrawCube(unsigned int vaoNum)
	{
		glBindVertexArray(vaoNum);

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
