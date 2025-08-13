#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test_shader.h"
#include "quaternion.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	float vertices[] = {
		// positions // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
	};

	float cube_vertices[] = {
		//top face
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // forward right
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,//forward left
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// back right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,//back left

		//bottom face
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,// bottom left
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left

		//front face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top right
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,//top left

		//back face
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// top right
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,//top left

		//right face
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top right
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// top right
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// bottom right

		//left face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,//top left
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f//top left

	};



	unsigned int indices[] = {
		//top
		0, 1, 2,
		2, 3, 1,

		//bottom
		4, 5, 6,
		6, 7, 5,

		//front
		8, 9, 10,
		10, 11, 8,

		//back
		12, 13, 14,
		14, 15, 12,

		//right
		16, 17, 18,
		18, 19, 17,

		//left
		20, 21, 22,
		22, 23, 21
	};

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	Quaternion Q = Quaternion::Quaternion(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(0.0f));
	//model = Q.GetRotationMatrix();
	//model = model * model;
	/*Quaternion new_q = Q.Multiply(Q, Q);
	model = new_q.GetRotationMatrix();*/

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << model[j][i] << " "; // Accessing in column-major order
		}
		std::cout << std::endl;
	}
	/*model = glm::rotate(model, glm::radians(270.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));*/
	/*model = model * model * model * model * model;*/
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	////model = glm::inverse(model);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//trans = glm::translate(trans, glm::vec3(0.25, 0, 0));

	//Create a vertex buuffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Create an element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	Shader ourShader("./vshader.txt", "./fshader.txt");

	
	//Create vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//Set the VAO as the active buffer (?)
	glBindVertexArray(VAO);

	//Set the VBO as the active buffer, and assign the vertices to it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)0);
	
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);

	ourShader.use();

	ourShader.setMat4("model", model);
	ourShader.setMat4("view", view);
	ourShader.setMat4("projection", proj);

	int count = 0;
	float theta = 1.0f;
	float total_angle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(VAO);

		/*float time = glfwGetTime();

		float z_shift = 0.1*sin(0.8f*time - 0.5f);

		std::cout << z_shift << std::endl;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, z_shift));*/
		//model = glm::rotate(model, glm::radians(1.0f), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
		ourShader.use();
		
		/*if (count < 10) {
			count += 1;
		}*/
		//else
		//{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (fabs(model[i][j]) < 1e-4f)
				{	
					model[i][j] = 0.0f;
				}
			}
		}

		count = 0;
		total_angle += theta;
		if (fabs(total_angle - 360.0f) < 1e-5f)
		{
			total_angle = 0.0f;
		}
		Q = Quaternion::Quaternion(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(total_angle));
		model = Q.GetRotationMatrix();

		ourShader.setMat4("model", model);
		//}


		//glm::mat4 correct_model = glm::inverse(model);

		
		//ourShader.setMat4("view", view);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}