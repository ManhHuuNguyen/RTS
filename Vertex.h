#pragma once
#include "glm\glm.hpp"

class Vertex {
	public:
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 textureCoord;
		glm::ivec4 jointIDs = glm::ivec4(0, 0, 0, 0);
		glm::vec4 weights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // always keep it sorted

		void addNewJoint(int jointID, float boneWeight);
};