#include "MovingUnit.h"




float MovingUnit::linearSpeed = 30.0f; // 30 world unit per second
float MovingUnit::angularSpeed = 1.57f; // pi radian per second
int MovingUnit::maxHealth = 50;

MovingUnit::MovingUnit(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale):Entity(model, orientation, position, scale) {

}

bool MovingUnit::updateVelocity(float * linearV, float * angularV, float linearSpeed, float angularSpeed, float elapsedSeconds) {
	if (desireLocation.hasADestination) {
		if (desireLocation.hasReachAngular(orientation.y)) {
			*angularV = 0.0f;
			glm::mat4 modelMat = getModelMat();
			BoundingBox updateBox = boundingBox.move(modelMat);
			if (!desireLocation.hasReachLinear(updateBox.minDimension, updateBox.maxDimension)) {
				*linearV = linearSpeed;
				return true;
			}
			else {
				*linearV = 0.0f;
				desireLocation.hasADestination = false;
				return false;
			}
		}
		else {
			*angularV = angularSpeed;
			return true;
		}
	}
	return false;
}

void MovingUnit::updatePosition(float linearV, float angularV, float elapsedSeconds) {
	float z = cos(orientation.y) * linearV * elapsedSeconds;
	float x = sin(orientation.y) * linearV * elapsedSeconds;
	float yAngle = angularV * elapsedSeconds * desireLocation.ccw;
	orientation.y = Util::normalizeRad(orientation.y);
	float supposedOrientation = orientation.y + yAngle;
	
	if (desireLocation.ccw == 1 && 
		Destination::angleOfError < (desireLocation.destinationAngle-orientation.y) && 
		supposedOrientation > desireLocation.destinationAngle) 
	{ 	
		yAngle = desireLocation.destinationAngle - orientation.y;
	}
	
	else if (desireLocation.ccw == -1 &&
			Destination::angleOfError < (orientation.y - desireLocation.destinationAngle) && 
			supposedOrientation < desireLocation.destinationAngle) 
	{	
		yAngle = desireLocation.destinationAngle - orientation.y;
	}

	if (Destination::angleOfError >= abs(desireLocation.destinationAngle - orientation.y)) {
		yAngle = desireLocation.destinationAngle - orientation.y;
	}
	else if (Destination::angleOfError >= abs(desireLocation.destinationAngle - supposedOrientation)) {
		yAngle = desireLocation.destinationAngle - orientation.y;
	}
	orientation.y += yAngle;
	glm::vec2 supposedPosition = glm::vec2(position.x + x, position.z + z);
	position.x += x;
	position.z += z;
}

void MovingUnit::moveToward(float x, float z) {
	desireLocation = Destination(position.x, position.z, x, z, orientation.y); // get the vector that we want to go to
}