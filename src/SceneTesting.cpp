#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "test_shader.h"
#include "quaternion.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "camera.h"
//#include "orbit_sim.h"
#include "scene.h"

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	

	glm::mat4 model = glm::mat4(1.0f);
	////model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	////model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	//Quaternion Q = Quaternion::Quaternion(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(45.0f));
	//model = Q.GetRotationMatrix();

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	Shader ourShader("shaders/vshader.txt", "shaders/fshader.txt");


	glEnable(GL_DEPTH_TEST);

	//glBindVertexArray(VAO);


	int frame = 0;

	

	ourShader.use();

	ourShader.setMat4("model", model);

	ourShader.setMat4("projection", proj);

	ourShader.setMat4("view", view);

	glm::vec3 sceneTarget = glm::vec3(0, 0, -1.0f);
	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0, 0, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	static Camera* cameraPtr = &camera;

	Scene scene("C:\\Users\\yairl\\Programs\\Graphics_and_Physics\\resources\\blender_monkey_suzanne.obj");
	
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {cameraPtr->checkIfMouseButtonPressed(window, button, action, mods);});

	float time = 0;
	float theta = 0.7f;
	float total_angle = 0.0f;


	while (!glfwWindowShouldClose(window))
	{
		ourShader.use();
		//input
		processInput(window);
		camera.processInput(window);
		glm::mat4 view = camera.GetCameraMatrix();
		ourShader.setMat4("view", view);

		

		if (camera.mousePressed) {
			camera.mouseButtonCallBack(window);
		}
		else {
			camera.getCurrMousePos(window);
		}

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		
	
		scene.drawScene(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}