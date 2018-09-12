#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) :origin{ origin }, direction{ direction } {
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