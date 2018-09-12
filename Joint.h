#pragma once
#include <vector>
#include "glm\glm.hpp"

class Joint {

	public:
		std::string name;
		std::vector<Joint> children;
		bool isAnimated = false;
		glm::mat4 localTransform; // this is the local transform relative to the joint's parent in bind pose
		glm::mat4 animatedTransform; // this will be updated every frame
		glm::mat4 inverseBindTransform;
		// this stays the same forever, this is the transform needed to
		// go from world space to bone space
		// the bind transform is the original transform of the joint in relation to the model's origin
};