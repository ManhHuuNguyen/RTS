#pragma once
#include "Entity.h"
class IntersectionRecord {
	public:
		Entity * e_ptr = nullptr;
		float hitDepth = std::numeric_limits<double>::infinity();
		glm::vec2 groundHitPoint = glm::vec2(2*CONSTANT::WORLD_SIZE, 2*CONSTANT::WORLD_SIZE); // point of impact with ground
		// set to an improbable value. if hit replace with proper value

};