#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(glm::vec3 & center, float radius) {
	this->center = center;
	this->radius = radius;
}

bool BoundingSphere::contains(BoundingSphere & b) {
	if (b.center.x + radius > center.x + radius) {
		return false;
	}
	if (b.center.x - radius < center.x - radius) {
		return false;
	}
	if (b.center.y + radius > center.y + radius) {
		return false;
	}
	if (b.center.y - radius < center.y - radius) {
		return false;
	}
	if (b.center.z + radius > center.z + radius) {
		return false;
	}
	if (b.center.z - radius < center.z - radius) {
		return false;
	}
	return true;
}

void BoundingSphere::move(glm::vec3 & movedVector) {
	center = center + movedVector;
}

BoundingSphere::BoundingSphere() {
	std::cout << "Bounding sphere default constructor is called, whatever" << std::endl;
}