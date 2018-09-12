#include "FollowCamera.h"

FollowCamera::FollowCamera(Entity * entity) {
	followedEntity = entity;
	glm::vec3 newLookVec = glm::normalize(glm::cross(up, right)) * distance;
	eyePosition = glm::vec4(followedEntity->position - newLookVec, 1.0f);
	normalizeLRU();
}

void FollowCamera::update() {
	glm::vec3 newLookVec = glm::normalize(glm::cross(up, right)) * distance;
	eyePosition = glm::vec4(followedEntity->position - newLookVec, 1.0f);
	normalizeLRU();
}

void FollowCamera::zoom(int direction) {
	distance += 1 * direction;
}