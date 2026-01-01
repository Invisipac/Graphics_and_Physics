#include "camera.h"
#include "quaternion.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startTarget, glm::vec3 startUp)
{
	this->cameraPosition = startPosition;
	this->cameraForward = startTarget;
	this->cameraUp = startUp;
	this->cameraSpeed = 0.05f;
	this->lastMouseX = 400.0f;
	this->lastMouseY = 300.0f;
	this->mousePressed = false;

}

glm::mat4 Camera::GetCameraMatrix()
{
	glm::mat4 view = glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraForward, this->cameraUp);

	return view;
}

void Camera::AutoMoveCamera()
{
	float time = glfwGetTime();

	this->cameraPosition = glm::vec3(cos(time)*10, 0.0f, sin(time)*10);
}

void Camera::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->moveCamera('w');
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->moveCamera('a');
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->moveCamera('s');
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->moveCamera('d');
	}


}

void Camera::checkIfMouseButtonPressed(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS){ this->mousePressed = true; }
		else if (action == GLFW_RELEASE) { this->mousePressed = false; }
	}
}
 

void Camera::mouseButtonCallBack(GLFWwindow* window) {
	
		double lastX = 0;
		double lastY = 0;
		double* pLastX = &lastX;
		double* pLastY = &lastY;
		glfwGetCursorPos(window, pLastX, pLastY); 
		
		//std::cout << "last x, y" << lastX << ':' << lastY << "curX, Y" << this->lastMouseX << ':' << this->lastMouseY << '\n';
		/*this->getCurrMousePos(window);*/
		if (abs(lastX - this->lastMouseX) > 10 || abs(lastY - this->lastMouseY) > 10) {
			glfwGetCursorPos(window, pLastX, pLastY);
			this->rotateCamera(lastX, lastY);
		}
}

void Camera::getCurrMousePos(GLFWwindow* window) {
	double lastX = 0;
	double lastY = 0;
	double* pLastX = &lastX;
	double* pLastY = &lastY;
	glfwGetCursorPos(window, pLastX, pLastY);
	this->lastMouseX = lastX;
	this->lastMouseY = lastY;
}

void Camera::moveCamera(char key) {
	if (key == 'w') {
		this->cameraPosition += this->cameraSpeed * this->cameraUp;// this->cameraForward;
	}
	else if (key == 'a') {
		this->cameraPosition -= this->cameraSpeed * glm::cross(this->cameraForward, this->cameraUp);
	}
	else if (key == 's') {
		this->cameraPosition -= this->cameraSpeed * this->cameraUp;// this->cameraForward;
	}
	else if (key == 'd') {
		this->cameraPosition += this->cameraSpeed * glm::cross(this->cameraForward, this->cameraUp);
	}
}


void Camera::rotateCamera(double xPos, double yPos) {
	float xDiff = xPos - this->lastMouseX;
	float yDiff = yPos - this->lastMouseY;

	glm::vec3 diffVec = glm::vec3(xDiff, yDiff, 0);

	this->lastMouseX = xPos;
	this->lastMouseY = yPos;

	glm::vec3 rotationAxis = glm::vec3(-yDiff, xDiff, 0);

	float diffMag = 0.1f*glm::length(diffVec);
	float radiusMag = glm::length(this->cameraPosition);
	/*float a = (diffMag, 2);
	float b = pow(radiusMag, 2);*/
	//float c = (pow(diffMag, 2) / (2 * pow(radiusMag, 2)));
	float val = 1 - (pow(diffMag, 2) / (2 * pow(radiusMag, 2)));
	float rotationAngle = acos(val);
	glm::mat4 id = glm::mat4(1.0f);
	glm::mat3 rotationMatrix = glm::mat3(glm::rotate(id, rotationAngle, rotationAxis));//glm::mat3(Quaternion::Rotate(rotationAxis, rotationAngle));

	glm::vec3 oldCam = this->cameraPosition;
	this->cameraPosition = rotationMatrix * this->cameraPosition;
	glm::vec3 camDiff = this->cameraPosition - oldCam;
	std::cout << this->cameraPosition.x << ':' << this->cameraPosition.y << ':' << this->cameraPosition.y  << '\n';
	/*if (this->cameraPosition.z <= 0 && oldCam.z > 0 && this->cameraPosition.x == oldCam.x) {
		this->cameraUp *= -1.0f;
	}*/
	this->cameraForward = glm::normalize(glm::vec3(0, 0, 0) - this->cameraPosition);

}
