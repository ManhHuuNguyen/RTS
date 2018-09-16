#include "Worker.h"


float Worker::linearSpeed = 10.0f; // 30 world unit per second
float Worker::angularSpeed = 2*CONSTANT::MT_PI; // pi radian per second
int Worker::maxHealth = 50;
int Worker::ID = ID::WORKER_ID;

Worker::Worker(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale):MovingUnit(model, orientation, position, scale) {

}

bool Worker::update(float elapsedSeconds) { // true if unit moves or rotates, false otherwise
	bool vChange = updateVelocity(&linearV, &angularV, linearSpeed, angularSpeed, elapsedSeconds);
	if (vChange) {
		animator.currentAnim = "untitled0";
	}
	else {
		animator.currentAnim = "";
	}
	updatePosition(linearV, angularV, elapsedSeconds);
	return vChange;
}

void Worker::handleEvents(InputWrapper & inputWrapper) {
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		if (inputWrapper.inputs[i].fromMouse) {
			if (inputWrapper.inputs[i].inputID == InputManager::RIGHT_PRESS) {
				moveToward(inputWrapper.inputs[i].floatRanges[0], inputWrapper.inputs[i].floatRanges[1]);
			}
		}
		else {
			switch (inputWrapper.inputs[i].inputID) {
			case SDLK_b:
				std::cout << "Worker is building something" << std::endl;
				break;
			case SDLK_c:
				std::cout << "Worker cancels building" << std::endl;
				break;
			}
		}
	}
}