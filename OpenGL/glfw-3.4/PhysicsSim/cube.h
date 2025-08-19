#pragma once
#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>


class Cube : public ThreeDShape {
public:
	float s;
	std::vector<int> cubeIndices;

	void SetCubeVertices();

	void GenIndices();

	void CreateCubeBuffer(unsigned int* vboNum);

	void CreateIndexBuffer(unsigned int* eboNum);

	void CreateAllBuffers(unsigned int* vboNum, unsigned int* eboNum, unsigned int* vaoNum);

	void DrawCube(unsigned int vaoNum);

};