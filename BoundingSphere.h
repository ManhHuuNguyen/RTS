#pragma once

#include "glm\glm.hpp"
#include <iostream>

class BoundingSphere {


	public:
		glm::vec3 center;
		float radius;

		BoundingSphere();
		BoundingSphere(glm::vec3 & center, float radius);
		bool contains(BoundingSphere & b);
		void move(glm::vec3 & movedVector);


};