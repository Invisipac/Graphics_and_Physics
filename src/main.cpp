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
#include "raytracer.h"
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0, 0.0, 20.0), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	//Scene scene("C:\\Users\\yairl\\Programs\\Graphics_and_Physics\\resources\\blender_monkey_suzanne.obj");
	RayTracer raytracer("C:\\Users\\yairl\\Programs\\Graphics_and_Physics\\resources\\blender_monkey_suzanne.obj");
	//glBindVertexArray(VAO);

	//raytracer.extractDataFromScene();
	//raytracer.setBuffers();
	raytracer.setShaders("shaders/quad_vert.glsl", "shaders/quad_frag.glsl", "shaders/raytrace.glsl");
	raytracer.runShaders();

	raytracer.setMat4("model", model);

	raytracer.setMat4("projection", proj);

	raytracer.setMat4("view", view);

	while (!glfwWindowShouldClose(window))
	{
		raytracer.runShaders();
		raytracer.dispatchShaders(800/16, 600/20, 1);
		raytracer.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}