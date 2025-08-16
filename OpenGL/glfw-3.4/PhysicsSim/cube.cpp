#include "3dshapes.h"
#include<glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>


class Cube : ThreeDShape {
	float s;

	void SetCubeVertices() {
		/*glm::vec3 ftl = */glm::vec3 relative[8] = { glm::vec3({ -s, s, s }),
			/*glm::vec3 ftr = */glm::vec3({ s, s, s }),
			/*glm::vec3 fbl = */glm::vec3({ -s, s, -s }),
			/*glm::vec3 fbr = */glm::vec3({ s, s, -s }),
			/*glm::vec3 btl = */glm::vec3({ -s, -s, s }),
			/*glm::vec3 btr = */glm::vec3({ s, -s, s }),
			/*glm::vec3 bbl = */glm::vec3({ -s, -s, -s }),
			/*glm::vec3 bbr = */glm::vec3({ s, -s, -s }) };

		for(int i = 0; i < 8; i++)
		{
			this->vertices.push_back(this->position + relative[i]);
		}



	}

};