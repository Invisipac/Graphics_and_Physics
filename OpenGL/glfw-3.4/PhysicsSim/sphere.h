#pragma once
#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>


class Sphere : public ThreeDShape {

	int sectors;
	int stacks;
public:

	Sphere(int sectors, int stacks);

	void GenSphereVertices(float r);

	void GenSphereIndices();
};