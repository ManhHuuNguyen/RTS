#include "Worker.h"


float Worker::linearSpeed = 10.0f; // 30 world unit per second
float Worker::angularSpeed = 2*CONSTANT::MT_PI; // pi radian per second
int Worker::maxHealth = 50;

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
	for (int i = 0; i < inputWrapper.keyActions.size(); i++) {
		switch (inputWrapper.keyActions[i]) {
		case SDLK_b:
			std::cout << "Worker is building something" << std::endl;
			break;
		case SDLK_c:
			std::cout << "Worker cancels building" << std::endl;
			break;
		}
		
	}
	int counter = 0;
	while (counter < inputWrapper.mouseActions.size()) {
		int actionType = inputWrapper.mouseActions[counter];
		if (actionType == InputManager::DRAG) {
			counter += 5;
		}
		else if (actionType == InputManager::LEFT_PRESS) {
			counter += 2;
		}
		else if (actionType == InputManager::RIGHT_PRESS) {
			counter += 2;
		}
		else if (actionType == InputManager::CURRENT_POS) {
			counter += 2;
		}
		counter += 1;
	}
}