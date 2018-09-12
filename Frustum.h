#pragma once
#include "Plane.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

class Frustum {

	public:
		Plane planes[6];

		Frustum(Camera & cam);
		bool pointInFrustum(glm::vec3 & point);
		bool boxInFrustum(BoundingBox & box);
		bool sphereInFrustum(BoundingSphere & sphere);

};