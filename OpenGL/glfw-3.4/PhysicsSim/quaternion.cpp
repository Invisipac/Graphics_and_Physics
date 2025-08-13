#define _USE_MATH_DEFINES
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "quaternion.h"
#include<cmath>

Quaternion::Quaternion(glm::vec4 vec)
{
	Q = glm::normalize(vec);
}

Quaternion::Quaternion(glm::vec3 axis, float angle)
{
	if (glm::length(axis) > 1) {
		axis = glm::normalize(axis);
		//std::cout << 'a' << std::endl;
	}

	//std::cout << glm::length(axis) << std::endl;

	float theta = angle / 2.0f;

	float real = cos(theta);
	float img = sin(theta);


	float qr = real;
	
	glm::vec3 v = img * axis;

	glm::vec4 result = glm::vec4(real, v);



	/*std::cout << real << ' ';*/

	/*for (int i = 0; i < 4; i++)
	{
		std::cout << result[i] << ' ';
	}*/

	/*std::cout << std::endl;*/

	Q = result;
}

glm::mat4 Quaternion::GetRotationMatrix()
{
	//if (glm::length(this->Q) > 1) {
	//	this->Q = glm::normalize(this->Q);
	//	//std::cout << 'a' << std::endl;
	//}

	float qr = this->Q.x;
	float qi = this->Q.y;
	float qj = this->Q.z;
	float qk = this->Q.w;

	float s = 1 / (glm::pow(glm::length(this->Q), 2));

	//std::cout << qr << ' ' << qi << ' ' << qj << ' ' << qk << ' ' << std::endl;

	//std::cout << s << std::endl;

	//std::cout << glm::length(this->Q) << std::endl;

	float mat4[16] =
	{
		1 - 2 * s * (qj * qj + qk * qk), 2 * s * (qi * qj - qk * qr)   , 2 * s * (qi * qk + qj * qr)    , 0.0f,
		2 * s * (qi * qj + qk * qr)    ,1 - 2 * s * (qi * qi + qk * qk), 2 * s * (qj * qk - qi * qr)    , 0.0f,
		2 * s * (qi * qk - qj * qr)    , 2 * s * (qj * qk + qi * qr)   , 1 - 2 * s * (qi * qi + qj * qj), 0.0f,
		0.0f                              , 0.0f                             , 0.0f                              , 1.0f                              
	};

	for (int i = 0; i < 16; i++)
	{
		if (fabs(mat4[i]) < 1e-4f)
		{
			mat4[i] = 0;
		}
	}

	glm::mat4 rotation = glm::make_mat4(mat4);

	return rotation;

}

glm::mat4 Quaternion::Compose(glm::mat4 leftMat, glm::mat4 rightMat) {
	return leftMat * rightMat;
}

Quaternion Quaternion::Multiply(Quaternion q1, Quaternion q2) {
	glm::vec4 x1 = q1.Q;
	glm::vec4 x2 = q2.Q;

	float r1 = x1.x;
	float r2 = x2.x;

	glm::vec3 v1 = glm::vec3(x1.y, x1.z, x1.w);
	glm::vec3 v2 = glm::vec3(x2.y, x2.z, x2.w);
	float real = r1 * r2 - glm::dot(v1, v2);
	glm::vec3 img = r1 * v2 + r2 * v1 + glm::cross(v1, v2);

	return Quaternion::Quaternion(glm::vec4(real, img));
}