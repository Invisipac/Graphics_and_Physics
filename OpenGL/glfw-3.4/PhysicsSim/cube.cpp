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
		Vertex{glm::vec3(-s, s, s), glm::vec3(1, 0, 0)}, //front top left
		Vertex{glm::vec3(s, s, s), glm::vec3(1, 0, 0)}, //front top right
		Vertex{ glm::vec3(-s, -s, s), glm::vec3(1, 0, 0)}, //front bottom left
		Vertex{glm::vec3(s, -s, s), glm::vec3(1, 0, 0)}, //front bottom right
		//Right
		Vertex{glm::vec3(s, s, s), glm::vec3(0, 1, 0)}, //front top right
		Vertex{glm::vec3(s, -s, s), glm::vec3(0, 1, 0)}, //front bottom right
		Vertex{glm::vec3(s, s, -s) , glm::vec3(0, 1, 0)}, //back top right
		Vertex{glm::vec3(s, -s, -s), glm::vec3(0, 1, 0)}, //back bottom right
		//Left
		Vertex{ glm::vec3(-s, s, s), glm::vec3(0, 0, 0) }, //front top left
		Vertex{ glm::vec3(-s, -s, s), glm::vec3(0, 0, 0)}, //front bottom left
		Vertex{glm::vec3(-s, s, -s), glm::vec3(0, 0, 0)}, //back top left
		Vertex{ glm::vec3(-s, -s, -s), glm::vec3(0, 0, 0) },//back bottom left
		//Top
		Vertex{ glm::vec3(-s, s, s), glm::vec3(0, 0, 1) }, //front top left
		Vertex{ glm::vec3(s, s, s), glm::vec3(0, 0, 1) }, //front top right
		Vertex{ glm::vec3(-s, s, -s), glm::vec3(0, 0, 1) }, //back top left
		Vertex{ glm::vec3(s, s, -s) , glm::vec3(0, 0, 1) }, //back top right
		//Bottom
		Vertex{ glm::vec3(-s, -s, s), glm::vec3(0, 0, 0)}, //front bottom left
		Vertex{glm::vec3(s, -s, s), glm::vec3(0, 0, 0)}, //front bottom right
		Vertex{ glm::vec3(-s, -s, -s), glm::vec3(0, 0, 0) },//back bottom left
		Vertex{ glm::vec3(s, -s, -s), glm::vec3(0, 0, 0) }, //back bottom right
		//Back
		Vertex{glm::vec3(-s, s, -s), glm::vec3(0, 0, 0)}, //back top left
		Vertex{glm::vec3(s, s, -s) , glm::vec3(0, 0, 0)}, //back top right
		Vertex{glm::vec3(-s, -s, -s), glm::vec3(0, 0, 0)},//back bottom left
		Vertex{glm::vec3(s, -s, -s), glm::vec3(0, 0, 0)} //back bottom right	
		}; 
	}

	void Cube::GenIndices()
	{
		for (int i = 0; i <= this->vertices.size() - 4; i += 4)
		{
			this->cubeIndices.push_back(i);
			this->cubeIndices.push_back(i + 1);
			this->cubeIndices.push_back(i + 2);

			this->cubeIndices.push_back(i + 1);
			this->cubeIndices.push_back(i + 2);
			this->cubeIndices.push_back(i + 3);
		}
	}

	void Cube::CreateCubeBuffer(unsigned int* vboNum)
	{
		glGenBuffers(1, vboNum);
		glBindBuffer(GL_ARRAY_BUFFER, *vboNum);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	}

	void Cube::CreateIndexBuffer(unsigned int* eboNum)
	{

		glGenBuffers(1, eboNum);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboNum);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->cubeIndices.size() * sizeof(int), &this->cubeIndices[0], GL_STATIC_DRAW);
	}
	
	void Cube::CreateAllBuffers(unsigned int* vboNum, unsigned int* eboNum, unsigned int* vaoNum)
	{
		glGenVertexArrays(1, vaoNum);
		glBindVertexArray(*vaoNum);

		this->CreateCubeBuffer(vboNum);
		this->CreateIndexBuffer(eboNum);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, colour));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void Cube::DrawCube(unsigned int vaoNum)
	{
		glBindVertexArray(vaoNum);

		glDrawElements(GL_TRIANGLES, this->cubeIndices.size(), GL_UNSIGNED_INT, 0);
	}
