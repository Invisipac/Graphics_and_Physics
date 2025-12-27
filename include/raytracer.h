#pragma once
#include <iostream>
#include<glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "test_shader.h"
#include "scene.h"
class RayTracer {
	Scene scene;
	unsigned int verticesBuffer, texBuffer, indicesBuffer, quadBuffer, quadVao, posVao, indicesVao, shaderID, computeShaderID;
	std::vector <glm::vec4> positions;
	std::vector <unsigned int> indices;
public:
	RayTracer(std::string path);
	void extractDataFromScene();
	void setBuffers();
	void setShaders(const char* vertexPath, const char* fragmentPath, const char* computePath);
	void runShaders();
	void render();
	void setMat4(const std::string& name, glm::mat4 matrix);
	void dispatchShaders(unsigned int workGroupX, unsigned int workGroupY, unsigned int workGroupZ);
};