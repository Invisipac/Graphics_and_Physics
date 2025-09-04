#pragma once
#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>


class Sphere :ThreeDShape {

	int sectors;
	int stacks;

	void GenSphereVertices(float r);

	void GenSphereIndices();
};