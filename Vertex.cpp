#include "Vertex.h"

void Vertex::addNewJoint(int jointID, float boneWeight) {
	if (boneWeight >= weights.w) {
		jointIDs.x = jointIDs.y;
		weights.x = weights.y;

		jointIDs.y = jointIDs.z;
		weights.y = weights.z;

		jointIDs.z = jointIDs.w;
		weights.z = weights.w;
		
		jointIDs.w = jointID;
		weights.w = boneWeight;
	}
	else if (boneWeight >= weights.z) { // < w and >= z
		jointIDs.x = jointIDs.y;
		weights.x = weights.y;

		jointIDs.y = jointIDs.z;
		weights.y = weights.z;

		jointIDs.z = jointID;  
		weights.z = boneWeight; 
	}
	else if (boneWeight >= weights.y) { // < z and >= y
		jointIDs.x = jointIDs.y;
		weights.x = weights.y;

		jointIDs.y = jointID;
		weights.y = boneWeight;
	}

	else  if (boneWeight > weights.x) { // < y and > x
		jointIDs.x = jointID;
		weights.x = boneWeight;
	}
}