#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test_shader.h"
#include "quaternion.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "cube.h"
#include "3dshapes.h"
#include "camera.h"
#include "sphere.h"

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

	glm::mat4 model = glm::mat4(1.0f);
	////model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	////model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	Quaternion Q = Quaternion::Quaternion(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(45.0f));
	model = Q.GetRotationMatrix();

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	Shader ourShader("./vshader.txt", "./fshader.txt");


	glEnable(GL_DEPTH_TEST);

	//glBindVertexArray(VAO);


	int frame = 0;

	Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

	ourShader.use();

	ourShader.setMat4("model", model);
	ourShader.setMat4("view", view);
	ourShader.setMat4("projection", proj);

	float theta = 0.7f;
	float total_angle = 0.0f;


	GLuint vboNum;
	GLuint eboNum;
	GLuint vaoNum;

	/*Cube TestCube;
	TestCube.s = 0.5f;
	TestCube.shapePosition = glm::vec3(0, 0, 0);
	TestCube.velocity = glm::vec3(0.05f, 0.05f, 0.05f);
	TestCube.acceleration = glm::vec3(0, -0.00981f, 0);
	TestCube.SetCubeVertices();
	TestCube.GenIndices();
	TestCube.CreateAllBuffers(&vboNum, &eboNum, &vaoNum);*/

	Sphere TestSphere(20, 20);
	TestSphere.GenSphereVertices(1);
	TestSphere.GenSphereIndices();
	TestSphere.CreateAllBuffers(&vboNum, &eboNum, &vaoNum);
	TestSphere.shapePosition = glm::vec3(0, 0, 0);
	TestSphere.velocity = glm::vec3(0.1f, 0.05f, 0.05f);
	TestSphere.acceleration = glm::vec3(0, -0.00981f, 0);
	//TestCube.CreateCubeBuffer(&vboNum);
	//TestCube.CreateIndexBuffer(&eboNum);

	float time = 0;

	glm::mat4 identity = glm::mat4(1.0f);

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glBindVertexArray(VAO);

		ourShader.use();


		total_angle += theta;
		if (fabs(total_angle - 360.0f) < 1e-5f)
		{
			total_angle = 0.0f;
		}
		model = Quaternion::Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(total_angle));
	
		////TestCube.Transform(model);

		////glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		////glBindVertexArray(0);
		//
		if (glfwGetTime() - time >= 1.0f / 80.0f)
		{
			TestSphere.MoveShape(10, frame);
			ourShader.setVec3("displacement", TestSphere.shapePosition);

			time = glfwGetTime();
		}

		///*camera.AutoMoveCamera();
		//view = camera.GetCameraMatrix(glm::vec3(0, 0, 0));*/

		ourShader.setMat4("model", model);
		////ourShader.setMat4("view", view);
		TestSphere.CheckCollision();
		//for (int i = 0; i < 1; i++)
		//{
		//	model = glm::translate(model, glm::vec3(i+0.2f, 0, 0));
		//	ourShader.setMat4("model", model);
		//	TestCube.DrawCube(vaoNum);
		//}

		TestSphere.DrawShape(vaoNum);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}