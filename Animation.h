#pragma once
#include "glm\glm.hpp"
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include<glm/common.hpp>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <vector>
#include <iostream>

// pugixml loader
#include "Util.h"
#include "map"

class PositionKeyFrame {
	
	public:
		glm::vec4 position;
		float timeStamp;
};

class RotationKeyFrame {

	public:
		glm::quat rotation;
		float timeStamp;
};

class ScaleKeyFrame {

public:
	glm::vec3 scale;
	float timeStamp;
};
class JointAnimation {
	public:
		std::vector<PositionKeyFrame> posKeyframes;
		std::vector<RotationKeyFrame> rotKeyframes;
		std::vector<ScaleKeyFrame> scaleKeyframes;
};

class Animation {
	public:
		float animationTimeLength;
		std::map<std::string, JointAnimation> jointAnimations;

		std::vector<PositionKeyFrame> getPreviousAndNextPos(float currentTime, JointAnimation & j);
		std::vector<RotationKeyFrame> getPreviousAndNextRot(float currentTime, JointAnimation & j);
		std::vector<ScaleKeyFrame> getPreviousAndNextScale(float currentTime, JointAnimation & j);
};