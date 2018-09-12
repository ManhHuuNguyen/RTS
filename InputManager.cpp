#include "InputManager.h"

int InputManager::DRAG = 2;
int InputManager::LEFT_PRESS = -1;
int InputManager::RIGHT_PRESS = 1;
int InputManager::CURRENT_POS = 0;


void MouseAction::pressLeft(int x, int y) {
	left = true;
	clickCoordX = x;
	clickCoordY = y;
}

void MouseAction::pressRight(int x, int y) {
	right = true;
	clickCoordX = x;
	clickCoordY = y;
}

bool MouseAction::isDrag() {
	if (!started){
		return false;
	}
	if (abs(startX - endX) < 5 && abs(startY - endY) < 5) {
		return false;
	}
	return true;
}

void MouseAction::start(int startX, int startY) {
	this->startX = startX;
	this->startY = startY;
	this->endX = startX;
	this->endY = startY;
	started = true;
}

void MouseAction::reset() {
	startX = -1;
	startY = -1;
	endX = -1;
	endY = -1;
	finished = false;
	started = false;

	left = false;
	right = false;
	clickCoordX = -1;
	clickCoordY = -1;
}

void MouseAction::update(int relX, int relY) {
	if (started) {
		endX += relX;
		endY += relY;
	}
}


InputWrapper InputManager::convertToActions(std::vector<SDL_Event> & events) {
	InputWrapper wrapper;
	mouseAction.pressed = false;
	for (SDL_Event & sdlEvent : events) {
		switch (sdlEvent.type){
			case SDL_KEYDOWN:
				wrapper.keyActions.push_back(sdlEvent.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					mouseAction.start(sdlEvent.motion.x, sdlEvent.motion.y);
					if (!mouseAction.left) {
						mouseAction.pressLeft(sdlEvent.motion.x, sdlEvent.motion.y);
						mouseAction.pressed = true;
					}
				}
				else if (sdlEvent.button.button == SDL_BUTTON_RIGHT){
					if (!mouseAction.right) {
						mouseAction.pressRight(sdlEvent.motion.x, sdlEvent.motion.y);
						mouseAction.pressed = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				mouseAction.reset();
				break;
			case SDL_MOUSEMOTION:
				if (sdlEvent.motion.state & SDL_BUTTON_LMASK) {
					mouseAction.update(sdlEvent.motion.xrel, sdlEvent.motion.yrel);					
				}
				break;
		}
	}
	if (mouseAction.isDrag()) { // DRAG, startX, startY, endX, endY, finished (0 or 1)
		wrapper.mouseActions.push_back(DRAG);
		wrapper.mouseActions.push_back(mouseAction.startX);
		wrapper.mouseActions.push_back(mouseAction.startY);
		wrapper.mouseActions.push_back(mouseAction.endX);
		wrapper.mouseActions.push_back(mouseAction.endY);
		wrapper.mouseActions.push_back(mouseAction.finished);
	}
	else if (mouseAction.pressed) { // LEFT or RIGHT, x, y
		if (mouseAction.left) {
			wrapper.mouseActions.push_back(LEFT_PRESS);
		}
		else if (mouseAction.right) {
			wrapper.mouseActions.push_back(RIGHT_PRESS);
		}
		wrapper.mouseActions.push_back(mouseAction.clickCoordX);
		wrapper.mouseActions.push_back(mouseAction.clickCoordY);
	}
	int posX, posY;
	SDL_GetMouseState(&posX, &posY);
	wrapper.mouseActions.push_back(CURRENT_POS); // CURRENT_POS then final x, y mouse position in this frame
	wrapper.mouseActions.push_back(posX);
	wrapper.mouseActions.push_back(posY);
	return wrapper;
}

