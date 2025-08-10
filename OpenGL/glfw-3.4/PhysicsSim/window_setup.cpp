#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test_shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
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

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		//Cycle green value using uniform attribute in fragment shader
		float time = glfwGetTime();
		float greenValue = (sin(time) / 2.0f) + 0.5f;


		


		//ourShader.setFourFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		


		//Set the VAO as the current object/buffer (?)
		
		glBindVertexArray(VAO);

		//Draw the rectangle

		trans = glm::rotate(trans, glm::radians(0.5f), glm::vec3(1.0, 0.0, 0.0));

		ourShader.use();
		
		ourShader.setMat4("transform", trans);



		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//Unbind (?)
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}