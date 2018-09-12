#pragma once
#include "glm\glm.hpp"
#include <iostream>
#include "Util.h"

/*
the official axis which any angle will be compared with is the positive z axis (0.0f, 0.0f, 1.0f)
*/

class Destination {

public:
	bool hasADestination = false;
	float destinationX, destinationZ; // destination
	float destinationAngle; // destination agle radians
	int ccw = 1; // turn counter clock wise or turn clock wise

	static float radiusOfArrival; // an entity arrive at a position within the is considered arrived at the destination
								  // this is to counter the effect of entity trying to arrive at the exact spot and end up moving back and forth uselessly
	static float angleOfError; // if an entity rotates to an orientation that is within this range of angleOfError, it is considered complete

	Destination();
	Destination(float startX, float startZ, float destinationX, float destinationZ, float currentYOrientation);
	bool hasReachLinear(glm::vec3 & min, glm::vec3 & max); // if object has reached the destination
	bool hasReachAngular(float orientationY); // if object has reach the angle needed to move
};
