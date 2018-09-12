#pragma once
#include "Animation.h"
#include <map>
#include <set>
#include "Joint.h"
#include "Util.h"
#include "CONSTANTS.h"
#include "Model.h"

/*
	There will be multiple animator, but their member variables are just pointers pointing to the same copy of the model
	It saves space this way and I dont have to have multiple copies
**/
class Animator {

	public:
		std::map<std::string, Animation> * animationMap;
		int animatedJointsNum;
		Joint rootJoint; 
		float animationCurrentTime = 0.0f;
		std::string currentAnim;

		Animator(Model * model);
				
		std::vector<glm::mat4> updateAnimation(float elapsedSeconds);
		void updateAnimationTime(float elapsedSeconds, Animation & currentAnimation);
		glm::mat4 interpolatePoses(std::vector<PositionKeyFrame> & pos, std::vector<RotationKeyFrame> & rot, std::vector<ScaleKeyFrame> & scal);
		void updateBoneTransforms(Joint & rootJoint, glm::mat4 & toWorldMatrix, Animation & currentAnimation, std::vector<glm::mat4> & boneTransforms);
};

