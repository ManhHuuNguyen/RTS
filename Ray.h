#pragma once
#include "glm\glm.hpp"
#include "Plane.h"
#include "IntersectionRecord.h"


class Ray {
	public:
		
		glm::vec3 origin;
		glm::vec3 direction;
		glm::vec3 invDir;

		Ray(glm::vec3 origin, glm::vec3 direction);
		void intersectPlane(Plane p, IntersectionRecord & intersectionRecord);

};
