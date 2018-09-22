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
		void intersectPlane(Plane & p, IntersectionRecord & intersectionRecord); // im not returning intersection record bc im using this method in a recursive method. so it works better this way
		static Ray getMouseRay(int x, int y, Camera * camera);
		static Ray getMouseRay(int x, int y, glm::mat4 & inverseViewMat, glm::vec4 & camPos);
};

