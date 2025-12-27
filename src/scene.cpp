#include "scene.h"


Scene::Scene(std::string path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	this->loadMeshes(scene->mRootNode, scene);
}

void Scene::loadMeshes(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->initMesh(aiMesh));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		this->loadMeshes(node->mChildren[i], scene);
	}
}


Mesh Scene::initMesh(aiMesh* mesh)
{
	std::vector <Vertex> paramVertices;
	std::vector <unsigned int> paramIndices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertex.colour = glm::vec3(0.2f, 0.2f, 0.2f);

		if (mesh->mTextureCoords[0]) {
			vertex.texture.x = mesh->mTextureCoords[0][i].x;
			vertex.texture.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.texture = glm::vec2(0.0f, 0.0f);
		}

		paramVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			paramIndices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(paramVertices, paramIndices);
}

void Scene::drawScene(Shader& shader) {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].drawMesh(shader);
	}
}

std::vector <Mesh> Scene::getMeshes() {
	return this->meshes;
}