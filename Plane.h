#pragma once

#include "glm\glm.hpp"
#include <vector>

class Plane {

	public:
		glm::vec3 point;
		glm::vec3 normal;

		Plane();
		Plane(glm::vec3 point, glm::vec3 normal);
		float distanceFromPoint(glm::vec3 point);
};