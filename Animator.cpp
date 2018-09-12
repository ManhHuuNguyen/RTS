#include "Animator.h"

Animator::Animator(Model * model) {
	animationMap = &(model->animationMap);
	rootJoint = model->rootJoint;
	animatedJointsNum = model->animatedJoints.size();
}


std::vector<glm::mat4> Animator::updateAnimation(float elapsedSeconds) {
	std::vector<glm::mat4> boneTransforms;
	if (animationMap == nullptr ||  animationMap->count(currentAnim) == 0) {
		for (int i = 0; i < animatedJointsNum+1; i++) { // to make sure that even unanimated models can have 1 bone->ensure Entity:loadUniformBoneToShader wont break down
			boneTransforms.push_back(CONSTANT::IDENTITY_MATRIX);
		}
	}
	else if (animationMap->count(currentAnim) != 0) {
		Animation & currentAnimation = (*animationMap)[currentAnim];
		updateAnimationTime(elapsedSeconds, currentAnimation);
		updateBoneTransforms(rootJoint, CONSTANT::IDENTITY_MATRIX, currentAnimation, boneTransforms);
	}
	return boneTransforms;
}


void Animator::updateAnimationTime(float elapsedSeconds, Animation & currentAnimation) {
	animationCurrentTime += elapsedSeconds;
	if (animationCurrentTime > currentAnimation.animationTimeLength) {
		animationCurrentTime = animationCurrentTime - currentAnimation.animationTimeLength;
	}
}

glm::mat4 Animator::interpolatePoses(std::vector<PositionKeyFrame> & pos, std::vector<RotationKeyFrame> & rot, std::vector<ScaleKeyFrame> & scal) {
	float deltaPosTime = pos[1].timeStamp - pos[0].timeStamp;
	float posProgression = (animationCurrentTime - pos[0].timeStamp) / deltaPosTime;
	glm::vec4 interpolatedPos = glm::mix(pos[0].position, pos[1].position, posProgression);

	float deltaRotTime = rot[1].timeStamp - rot[0].timeStamp;
	float rotProgression = (animationCurrentTime - rot[0].timeStamp) / deltaRotTime;
	glm::quat interpolatedRot = glm::slerp(rot[0].rotation, rot[1].rotation, rotProgression);

	float deltaScalTime = scal[1].timeStamp - scal[0].timeStamp;
	float scalProgression = (animationCurrentTime - scal[0].timeStamp) / deltaScalTime;
	glm::vec3 interpolatedScale = glm::mix(scal[0].scale, scal[1].scale, scalProgression);

	glm::mat4 translateMat = glm::translate(glm::vec3(interpolatedPos.x, interpolatedPos.y, interpolatedPos.z));
	return translateMat * glm::toMat4(interpolatedRot) * glm::scale(interpolatedScale);
}

// world pose here means the coordinate all the vertices are specified in, not the world coordinate of the game
void Animator::updateBoneTransforms(Joint & joint, glm::mat4 & toWorldMatrix, Animation & currentAnimation, std::vector<glm::mat4> & boneTransforms) {
	if (joint.isAnimated){
		std::vector<PositionKeyFrame> pos = currentAnimation.getPreviousAndNextPos(animationCurrentTime, currentAnimation.jointAnimations[joint.name]);
		std::vector<RotationKeyFrame> rot = currentAnimation.getPreviousAndNextRot(animationCurrentTime, currentAnimation.jointAnimations[joint.name]);
		std::vector<ScaleKeyFrame> scal = currentAnimation.getPreviousAndNextScale(animationCurrentTime, currentAnimation.jointAnimations[joint.name]);
		joint.localTransform = interpolatePoses(pos, rot, scal);
	}
	joint.animatedTransform = toWorldMatrix * joint.localTransform;
	if (joint.isAnimated) {
		boneTransforms.push_back(joint.animatedTransform * joint.inverseBindTransform);
	}
	for (int i = 0; i < joint.children.size(); i++) {
		updateBoneTransforms(joint.children[i], joint.animatedTransform, currentAnimation, boneTransforms);
	}
}
