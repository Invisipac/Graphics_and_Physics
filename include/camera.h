#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Camera {
	glm::vec3 cameraPosition;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;
	double lastMouseX, lastMouseY;
	float cameraSpeed;
	

public:
	bool mousePressed;

	Camera(glm::vec3 startPosition, glm::vec3 startForward, glm::vec3 startUp);

	void AutoMoveCamera();

	glm::mat4 GetCameraMatrix();

	void processInput(GLFWwindow* window);
	void moveCamera(char key);
	void rotateCamera(double xPos, double yPos);
	void mouseButtonCallBack(GLFWwindow* window);
	void getCurrMousePos(GLFWwindow* window);
	void checkIfMouseButtonPressed(GLFWwindow*, int button, int action, int mods);
};