#include "Destination.h"


float Destination::radiusOfArrival = 0.1f;
float Destination::angleOfError = 0.03f; // 1.7 degree

Destination::Destination() {
	destinationX = 0.0f;
	destinationZ = 0.0f;
	destinationAngle = 0.0f;
	hasADestination = false;
}

// i have documented the reason for this code inside my notebook. gotta keep track of my brain
Destination::Destination(float startX, float startZ, float destinationX, float destinationZ, float curYOrientation) {
	this->destinationX = destinationX;
	this->destinationZ = destinationZ;
	float vecX = destinationX - startX;
	float vecZ = destinationZ - startZ;
	glm::vec2 normVecXZ = glm::normalize(glm::vec2(vecX, vecZ));
	destinationAngle = std::acos(glm::dot(normVecXZ, glm::vec2(0.0f, 1.0f)));
	if (vecX < 0.0f) {
		destinationAngle = 2*CONSTANT::MT_PI-destinationAngle;
	}
	if (abs(destinationAngle-curYOrientation) < CONSTANT::MT_PI) {
		if (destinationAngle < curYOrientation) {
			ccw = -1;
		}
		else {
			ccw = 1;
		}
		
	}
	else {
		if (destinationAngle < curYOrientation) {
			ccw = 1;
		}
		else {
			ccw = -1;
		}
	}
	hasADestination = true;
}
bool Destination::hasReachLinear(glm::vec3 & min, glm::vec3 & max) {
	if (min.x <= destinationX && destinationX <= max.x &&
		min.z <= destinationZ && destinationZ <= max.z) {
		return true;
	}
	return false;
}

bool Destination::hasReachAngular(float orientationY) {
	return abs(destinationAngle-orientationY) <= CONSTANT::kEpsilon;
}
