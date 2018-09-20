#include "Worker.h"


float Worker::linearSpeed = 10.0f; // 30 world unit per second
float Worker::angularSpeed = 2*CONSTANT::MT_PI; // pi radian per second
int Worker::maxHealth = 50;

Worker::Worker(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale):MovingUnit(model, orientation, position, scale, ID::WORKER_ID) {

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

void Worker::handleEvents(std::vector<Action> & actions) {
	for (int i = 0; i < actions.size(); i++) {
		if (actions[i].fromMouse) {
			if (actions[i].key == InputManager::RIGHT_PRESS) {
				moveToward(actions[i].floatRanges[0], actions[i].floatRanges[1]);
			}
		}
		else {
			std::string key = actions[i].key;
			if (key == "B") {
				std::cout << "Worker is building something" << std::endl;
			}
			else if (key == "C") {
				std::cout << "Worker cancels building" << std::endl;
			}
			
		}
	}
}