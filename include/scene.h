#pragma once
#include <iostream>
#include<glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "test_shader.h"
#include "mesh.h"


class Scene {
	std::vector <Mesh> meshes;
	std::string directory;

public:
	Scene(std::string path);
	void loadMeshes(aiNode* node, const aiScene* scene);
	void drawScene(Shader &shader);
	Mesh initMesh(aiMesh* mesh);
	std::vector <Mesh> getMeshes();
};