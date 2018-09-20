#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) :origin{ origin }, direction{ direction } {
	invDir = 1.0f / direction;
}

Ray::Ray(glm::vec3 p1, glm::vec3 p2, bool isNormalized) : origin{p1} {
	direction = p2 - p1;
	if (!isNormalized) {
		direction = glm::normalize(direction);
	}
	invDir = 1.0f / direction;
}

void Ray::intersectPlane(Plane p, IntersectionRecord & intersectionRecord) {
	float denom = glm::dot(p.normal, direction); // because direction is already normalized in mouse
	if (!Util::nearZero(denom)) { // if dot product not zero
		glm::vec3 p0l0 = p.point - origin;
		float t = glm::dot(p0l0, p.normal) / denom;
		glm::vec3 hitVector = origin + t * direction;
		intersectionRecord.groundHitPoint = glm::vec2(hitVector.x, hitVector.z);
	}
	// if dot product = 0, paralell, either no hit or infinite hit when ray lies on plane
}

Ray Ray::getMouseRay(int x, int y, Camera * camera) { // just multiplying the ray with the inverse of matrices to get back to world coord
	float normalizedDeviceX = 2.0f * x / CONSTANT::WIDTH_DISPLAY - 1.0f;
	float normalizedDeviceY = 1.0f - 2.0f * y / CONSTANT::HEIGHT_DISPLAY;
	glm::vec4 rayClip = glm::vec4(normalizedDeviceX, normalizedDeviceY, -1.0f, 1.0f);
	glm::vec4 rayEye = CONSTANT::INV_PROJECTION_MATRIX * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(camera->getViewMatrix()) * rayEye));
	return Ray{ glm::vec3(camera->eyePosition), rayWorld };
}