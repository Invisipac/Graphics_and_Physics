#pragma once
#include <iostream>
#include<glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "test_shader.h"
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 texture;
};

class Mesh {
	std::vector <Vertex> vertices;
	std::vector <unsigned int> indices;
	unsigned int VAO, VBO, EBO;

public:
	Mesh(std::vector <Vertex> vertices, std::vector<unsigned int> indices);
	/*void initMesh(aiMesh* mesh);*/ //read from mesh file and load data into the vertices vector
	void setBuffers(); //setup opengl vertex, index, texture, and attribute buffers
	void drawMesh(Shader& shader); //bind and unbind the respective buffers, then call GL_DRAW
	std::vector <Vertex> getVertices();
	std::vector <unsigned int> getIndices();
};