#pragma once
#include "glm\glm.hpp"

struct Light {
	glm::vec4 intensity; // intensity doesn't need 4 slots, but do it this way to accomodate the stupid UniformBufferObject
	glm::vec4 position;
};