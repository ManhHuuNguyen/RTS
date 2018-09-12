#include "Animation.h"

std::vector<PositionKeyFrame> Animation::getPreviousAndNextPos(float currentTime, JointAnimation & j) {
	std::vector<PositionKeyFrame> prevAndNext;
	assert(j.posKeyframes.size() >= 2);
	PositionKeyFrame * prev = &(j.posKeyframes[0]); // use pointer to optimize, dont need to cp object. cant use reference here. 
	PositionKeyFrame * next = &(j.posKeyframes[0]);
	for (int i = 1; i < j.posKeyframes.size(); i++) {
		next = &(j.posKeyframes[i]);
		if (j.posKeyframes[i].timeStamp > currentTime) {
			break;
		}
		prev = &(j.posKeyframes[i]);
	}
	prevAndNext.push_back(*prev);
	prevAndNext.push_back(*next);
	return prevAndNext;

}
std::vector<RotationKeyFrame> Animation::getPreviousAndNextRot(float currentTime, JointAnimation & j) {
	std::vector<RotationKeyFrame> prevAndNext;
	assert(j.rotKeyframes.size() >= 2);
	RotationKeyFrame * prev = &(j.rotKeyframes[0]); // use pointer to optimize, dont need to cp object. cant use reference here. 
	RotationKeyFrame * next = &(j.rotKeyframes[0]);
	for (int i = 1; i < j.rotKeyframes.size(); i++) {
		next = &(j.rotKeyframes[i]);
		if (j.rotKeyframes[i].timeStamp > currentTime) {
			break;
		}
		prev = &(j.rotKeyframes[i]);
	}
	prevAndNext.push_back(*prev);
	prevAndNext.push_back(*next);
	return prevAndNext;

}
std::vector<ScaleKeyFrame> Animation::getPreviousAndNextScale(float currentTime, JointAnimation & j) {
	std::vector<ScaleKeyFrame> prevAndNext;
	assert(j.scaleKeyframes.size() >= 2);
	ScaleKeyFrame * prev = &(j.scaleKeyframes[0]); // use pointer to optimize, dont need to cp object. cant use reference here. 
	ScaleKeyFrame * next = &(j.scaleKeyframes[0]);
	for (int i = 1; i < j.scaleKeyframes.size(); i++) {
		next = &(j.scaleKeyframes[i]);
		if (j.scaleKeyframes[i].timeStamp > currentTime) {
			break;
		}
		prev = &(j.scaleKeyframes[i]);
	}
	prevAndNext.push_back(*prev);
	prevAndNext.push_back(*next);
	return prevAndNext;

}