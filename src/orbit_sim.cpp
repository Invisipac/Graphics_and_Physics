#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "sphere.h"
#include <math.h>
#include "3dshapes.h"
#include "orbit_sim.h"
#include <cmath>

	OrbitSim::OrbitSim(float time_step, float centralMass)
	:	centralBody(Sphere(20, 20)),
		orbitingBody(Sphere(20, 20))
	{
		//this->orbitalVelocity = velocity;
		this->dt = time_step;
		this->dtSquared = time_step * time_step;
		this->centralMass = centralMass;
		//this->orbitingMass = orbitingMass;
	}

	void OrbitSim::initBodies(glm::vec3 centralPos, glm::vec3 orbitingPos)
	{
		this->centralBody.shapePosition = centralPos;
		this->orbitingBody.shapePosition = orbitingPos;


		this->orbitalRadius = this->centralBody.shapePosition - this->orbitingBody.shapePosition;
		this->radiusMagnitude = glm::length(this->orbitalRadius);

		this->orbitalVelocity = glm::vec3(0, 0, sqrt((9.81f * this->centralMass / this->radiusMagnitude)));
	}
		
	void OrbitSim :: moveBody() {
		this->orbitalRadius = this->centralBody.shapePosition - this->orbitingBody.shapePosition;
		this->radiusMagnitude = glm::length(this->orbitalRadius);
		float accelMag = (float)((9.81f * this->centralMass) / pow(this->radiusMagnitude, 3));
		
		this->orbitalAcceleration = this->orbitalRadius * accelMag;
		this->orbitalVelocity = this->orbitalVelocity + this->orbitalAcceleration * dt;
		this->orbitingBody.shapePosition += this->orbitalVelocity * dt;// + 0.5f * this->orbitalAcceleration * this->dtSquared ;
	}

	void OrbitSim :: drawBodies(unsigned int vaoNum)
	{
		this->centralBody.DrawShape(vaoNum);
		this->orbitingBody.DrawShape(vaoNum);
	}

	glm::vec3 OrbitSim::getCentralPos()
	{
		return this->centralBody.shapePosition;
	}

	glm::vec3 OrbitSim::getOrbitingPos()
	{
		return this->orbitingBody.shapePosition;
	}

	Sphere OrbitSim::getCentralSphere()
	{
		return this->centralBody;
	}

	Sphere OrbitSim::getOrbitingSphere()
	{
		return this->orbitingBody;
	}