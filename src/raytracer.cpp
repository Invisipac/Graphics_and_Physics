#include "raytracer.h"



void RayTracer::extractDataFromScene() {
	//std::vector <glm::vec3> positions;
	//std::vector <unsigned int> indices;

	std::vector <Mesh> meshes = this->scene.getMeshes();

	std::cout << "=== MESH DEBUG ===" << std::endl;
	std::cout << "Number of meshes: " << meshes.size() << std::endl;

	int vertexOffset = 0;

	float meshIndexOffset = 0;
	for (unsigned int i = 0; i < meshes.size(); i++) {
		std::vector <Vertex> meshVertices = meshes[i].getVertices();
		std::vector <unsigned int> meshIndices = meshes[i].getIndices();

		std::cout << "Mesh " << i << ": "
			<< meshVertices.size() << " vertices, "
			<< meshIndices.size() << " indices"
			<< " (offset: " << vertexOffset << " to " << (vertexOffset + meshVertices.size() - 1) << ")"
			<< std::endl;

		// Print first vertex position
		if (meshVertices.size() > 0) {
			glm::vec3 pos = meshVertices[0].position;
			std::cout << "  First vertex:  (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
		}

		for (unsigned int j = 0; j < meshVertices.size(); j++) {
			//std::cout << "Pos " << j << "  " << meshVertices[j].position.x << " , " << meshVertices[j].position.y << " , " << meshVertices[j].position.z << std::endl;
			this->positions.push_back(glm::vec4(meshVertices[j].position, 1.0f));
			this->normals.push_back(glm::vec4(meshVertices[j].normal, 1.0f));
		}

		vertexOffset += meshVertices.size();

		for (unsigned int j = 0; j < meshIndices.size(); j++) {
			//std::cout << "idx " << meshIndices[j] << std::endl;
			this->indices.push_back(meshIndices[j] + meshIndexOffset);	
		}

		meshIndexOffset += meshVertices.size();
	}

	std::cout << "Total vertices: " << this->positions.size() << std::endl;
	std::cout << "==================" << std::endl;
}

void RayTracer::setBuffers() {
	glGenBuffers(1, &this->verticesBuffer);
	glGenBuffers(1, &this->indicesBuffer);
	glGenBuffers(1, &this->quadBuffer);
	glGenBuffers(1, &this->normalBuffer);

	// Make sure no other buffer is bound
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// VERTICES BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->verticesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->positions.size() * sizeof(glm::vec4), this->positions.data(), GL_STATIC_DRAW);

	// INDICES BUFFER  
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->indicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

	//NORMAL BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->normalBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->normals.size() * sizeof(glm::vec4), this->normals.data(), GL_STATIC_DRAW);

	// Unbind SSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Now bind to shader binding points
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->verticesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->indicesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->normalBuffer);

	// Don't create VAOs for SSBO buffers - you don't need them for compute shaders! 

	// IMAGE TEXTURE
	unsigned int tex_w = 800, tex_h = 600;
	glGenTextures(1, &this->texBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, this->texBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// QUAD FOR RENDERING
	float quadVertices[] = {
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	};

	glGenBuffers(1, &this->quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &this->quadVao);
	glBindVertexArray(this->quadVao);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void RayTracer::setShaders(const char* vertexPath, const char* fragmentPath, const char* computePath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string computeCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream computeShaderFile;
	// ensure ifstream objects can throw exceptions: 
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	computeShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		computeShaderFile.open(computePath);
		std::stringstream vShaderStream, fShaderStream, computeStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		computeStream << computeShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		computeShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		computeCode = computeStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR:: SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	std::cout << "=== VERTEX SHADER SOURCE ===" << std::endl;
	std::cout << vertexCode << std::endl;
	std::cout << "=== FRAGMENT SHADER SOURCE ===" << std::endl;
	std::cout << fragmentCode << std::endl;
	std::cout << "=== COMPUTE SHADER SOURCE ===" << std::endl;
	std::cout << computeCode << std::endl;
	std::cout << "=============================" << std::endl;

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* computeShaderCode = computeCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment, compute;

	int vsuccess;
	char vinfoLog[512];
	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vsuccess);
	if (!vsuccess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, vinfoLog);
		std::cout << "ERROR::SHADER::VERTEX:: COMPILATION_FAILED\n" <<
			vinfoLog << std::endl;
	};

	int fsuccess;
	char finfoLog[512];
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fsuccess);
	if (!fsuccess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, finfoLog);
		std::cout << "ERROR::SHADER:: FRAGMENT::COMPILATION_FAILED\n" <<
			finfoLog << std::endl;
	};

	int computeShaderSuccess;
	char computeShaderInfoLog[512];
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &computeShaderCode, NULL);
	glCompileShader(compute);

	glGetShaderiv(compute, GL_COMPILE_STATUS, &computeShaderSuccess);
	if (!computeShaderSuccess)
	{
		glGetShaderInfoLog(compute, 512, NULL, computeShaderInfoLog);
		std::cout << "ERROR::SHADER::COMPUTE:: COMPILATION_FAILED\n" <<
			computeShaderInfoLog << std::endl;
	};

	// CREATE SEPARATE PROGRAMS! 
	int psuccess;
	char pinfoLog[512];

	// Graphics pipeline program (vertex + fragment)
	this->shaderID = glCreateProgram();
	glAttachShader(this->shaderID, vertex);
	glAttachShader(this->shaderID, fragment);
	glLinkProgram(this->shaderID);
	glGetProgramiv(this->shaderID, GL_LINK_STATUS, &psuccess);
	if (!psuccess)
	{
		glGetProgramInfoLog(this->shaderID, 512, NULL, pinfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			pinfoLog << std::endl;
	}

	// Compute shader program (compute only)
	this->computeShaderID = glCreateProgram();
	glAttachShader(this->computeShaderID, compute);
	glLinkProgram(this->computeShaderID);
	glGetProgramiv(this->computeShaderID, GL_LINK_STATUS, &psuccess);
	if (!psuccess)
	{
		glGetProgramInfoLog(this->computeShaderID, 512, NULL, pinfoLog);
		std::cout << "ERROR::COMPUTE:: PROGRAM::LINKING_FAILED\n" <<
			pinfoLog << std::endl;
	}

	// delete shaders; they're linked into our program and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(compute);
}

void RayTracer::runShaders() {
	glUseProgram(this->computeShaderID);

	// Check if program is valid
	glValidateProgram(this->computeShaderID);
	GLint valid = 0;
	glGetProgramiv(this->computeShaderID, GL_VALIDATE_STATUS, &valid);
	if (!valid) {
		char log[512];
		glGetProgramInfoLog(this->computeShaderID, 512, NULL, log);
		std::cout << "ERROR: Compute program validation failed:\n" << log << std::endl;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->verticesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->indicesBuffer);
	glBindImageTexture(0, this->texBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// Check for any OpenGL errors
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error in runShaders: " << err << std::endl;
	}
	
}
void RayTracer::dispatchShaders(unsigned int workGroupX, unsigned int workGroupY, unsigned int workGroupZ) {
	glDispatchCompute(workGroupX, workGroupY, workGroupZ);
}

void RayTracer::render() {
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(this->shaderID);  // Use graphics program for rendering
	glBindVertexArray(this->quadVao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texBuffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void RayTracer::setMat4(const std::string& name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(this->computeShaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

RayTracer::RayTracer(std::string path):
	scene(Scene(path))
{
	this->extractDataFromScene();
	this->setBuffers();
}