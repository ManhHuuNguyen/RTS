#pragma once

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include <algorithm>
#include <iostream>
#include <vector>


class BoundingBox { //AABB

	public:
		// doing this because im confident that no model will have coordinate larger than 10000
		glm::vec3 minDimension = glm::vec3(10000.0f, 10000.0f, 10000.0f);
		glm::vec3 maxDimension = glm::vec3(-10000.0f, -10000.0f, -10000.0f);
		

		BoundingBox();
		BoundingBox(glm::vec3 & minDimension, glm::vec3 & maxDimension);
		BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
		BoundingBox(const BoundingBox & obj);
		
		bool contains(BoundingBox & b);
		bool inside(float xStart, float zStart, float xEnd, float zEnd); // compare with a 2d rectangle, to implement drag and select
		bool collide(BoundingBox & b);
		BoundingBox move(glm::mat4 & modelMatrix);
		
		glm::vec3 getEdgeSize();
		glm::vec3 getCenter();
		std::vector<glm::vec3> getCorners();

		
		
};