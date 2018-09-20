#pragma once
#include "Camera.h"
#include "glm\glm.hpp"
#include "Plane.h"
#include "IntersectionRecord.h"


class Ray {
	public:
		
		glm::vec3 origin;
		glm::vec3 direction;
		glm::vec3 invDir;

		Ray(glm::vec3 origin, glm::vec3 direction);
		Ray(glm::vec3 p1, glm::vec3 p2, bool isNormalized); // this bool is used to differentiate between two constructor
		void intersectPlane(Plane p, IntersectionRecord & intersectionRecord);
		static Ray getMouseRay(int x, int y, Camera * camera);
};

