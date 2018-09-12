#include "Frustum.h"


Frustum::Frustum(Camera & cam) {
	cam.normalizeLRU();
	glm::vec3 camPosition = glm::vec3(cam.eyePosition);
	
	glm::vec3 nc = camPosition + cam.look * CONSTANT::zNear; // near center
	glm::vec3 fc = camPosition + cam.look * CONSTANT::zFar; // far center
	Plane nearPlane{ nc, cam.look };
	Plane farPlane{ fc, -cam.look }; // we want all normals to point into the viewing volume to perform intersection test

	float hNear = 2 * CONSTANT::zNear * tanf(CONSTANT::fov / 2.0f); // measure in radian, fov is the full vertical
	float wNear = hNear * CONSTANT::aspectRatio;

	glm::vec3 vecToNearPlaneCenter = nc - camPosition;

	glm::vec3 vecOnRightPlane = vecToNearPlaneCenter + (cam.right * wNear / 2.0f);
	glm::vec3 rightPlaneNormal = glm::cross(cam.up, glm::normalize(vecOnRightPlane));
	Plane rightPlane{ camPosition, rightPlaneNormal };

	glm::vec3 vecOnLeftPlane = vecToNearPlaneCenter - (cam.right*wNear / 2.0f);
	glm::vec3 leftPlaneNormal = glm::cross(glm::normalize(vecOnLeftPlane), cam.up);
	Plane leftPlane{ camPosition, leftPlaneNormal };

	glm::vec3 vecOnTopPlane = vecToNearPlaneCenter + (cam.up * hNear / 2.0f);
	glm::vec3 topPlaneNormal = glm::cross(glm::normalize(vecOnTopPlane), cam.right);
	Plane topPlane{ camPosition, topPlaneNormal };

	glm::vec3 vecOnBottomPlane = vecToNearPlaneCenter - (cam.up * hNear / 2.0f);
	glm::vec3 bottomPlaneNormal = glm::cross(cam.right, glm::normalize(vecOnBottomPlane));
	Plane bottomPlane{ camPosition, bottomPlaneNormal };

	planes[0] = leftPlane;
	planes[1] = rightPlane;
	planes[2] = bottomPlane;
	planes[3] = topPlane;
	planes[4] = nearPlane;
	planes[5] = farPlane;
	
}

bool Frustum::pointInFrustum(glm::vec3 & point) {
	for (int i = 0; i < 6; i++) {
		if (planes[i].distanceFromPoint(point) < 0) {
			return false;
		}
	}
	return true;
}

bool Frustum::boxInFrustum(BoundingBox & box) {
	for (int i = 0; i < 6; i++) {
		glm::vec3 positiveVertex = box.minDimension;
		Plane & plane = planes[i];
		if (plane.normal.x >= 0.0f) {
			positiveVertex.x = box.maxDimension.x;
		}
		if (plane.normal.y >= 0.0f) {
			positiveVertex.y = box.maxDimension.y;
		}
		if (plane.normal.z >= 0.0f) {
			positiveVertex.z = box.maxDimension.z;
		}
		if (plane.distanceFromPoint(positiveVertex) < 0) {
			return false;
		}
	}
	return true;
}

bool Frustum::sphereInFrustum(BoundingSphere & sphere) {
	for (int i = 0; i < 6; i++) {
		float distance = planes[i].distanceFromPoint(sphere.center);
		if (distance < -sphere.radius) { // equivalent to if distance < 0 && abs(distance) > sphere.radius
			return false;
		}
	}
	return true;
}