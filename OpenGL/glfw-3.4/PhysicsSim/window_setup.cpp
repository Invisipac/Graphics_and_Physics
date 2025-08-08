#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test_shader.h"
#include <iostream>
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

	float rect_vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f //top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	//Create a vertex buuffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Create an element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Create and compile the vertex shader
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4 (aPos, 1.0);\n"
		" ourColor = aColor;\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Create and compile the fragment shader
	const char* fragmentShaderSource = "#version 330 core \n"
	"out vec4 FragColor; \n"
	"in vec3 ourColor; \n"
	"void main() \n"
	"{ \n"
	"	FragColor = vec4 (ourColor, 1.0); \n"
	"}\0";
	
	Shader ourShader("./vshader.txt", "./fshader.txt");

	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);


	////Link and create the shader program
	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);
	//glLinkProgram(shaderProgram);

	////Delete shaders now that the program is created
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);

	//Create vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//Set the VAO as the active buffer (?)
	glBindVertexArray(VAO);

	//Set the VBO as the active buffer, and assign the vertices to it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Set the EBO as the active buffer, and assign the indices to it
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);*/

	//Set the vertex attributes
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use the shader program
		//glUseProgram(shaderProgram);
		ourShader.use();

		//Cycle green value using uniform attribute in fragment shader
		float time = glfwGetTime();
		float greenValue = (sin(time) / 2.0f) + 0.5f;
		/*int vertexLocation = glGetUniformLocation(shaderProgram, "ourColor");

		

		glUniform4f(vertexLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

		ourShader.setFourFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

		//Set the VAO as the current object/buffer (?)
		glBindVertexArray(VAO);

		//Draw the rectangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Unbind (?)
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}