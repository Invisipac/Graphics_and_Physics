#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "sphere.h"
#include <math.h>
#include "3dshapes.h"


class OrbitSim {
	Sphere centralBody;
	Sphere orbitingBody;

	glm::vec3 orbitalRadius;
	glm::vec3 orbitalVelocity;
	glm::vec3 orbitalAcceleration;
	float dt;
	float dtSquared;

	float centralMass;
	float orbitingMass;

	float radiusMagnitude;

public:
	OrbitSim(float time_step, float centralMass);

	void initBodies(glm::vec3 centralPos, glm::vec3 orbitingPos);

	void moveBody();

	void drawBodies(unsigned int vaoNum);

	glm::vec3 getCentralPos();
	
	glm::vec3 getOrbitingPos();

	Sphere getCentralSphere();
	Sphere getOrbitingSphere();
};