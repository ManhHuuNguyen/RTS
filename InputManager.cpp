#include "InputManager.h"

int InputManager::DRAG = 3;
int InputManager::LEFT_PRESS = 1;
int InputManager::RIGHT_PRESS = 2;
int InputManager::CURRENT_POS = 0;
int InputManager::LEFT_UP = -1;
int InputManager::RIGHT_UP = -2;


void MouseInput::pressLeft(int x, int y) {
	left = true;
	clickCoordX = x;
	clickCoordY = y;
}

void MouseInput::pressRight(int x, int y) {
	right = true;
	clickCoordX = x;
	clickCoordY = y;
}

bool MouseInput::isDrag() {
	if (!started){
		return false;
	}
	if (abs(startX - endX) < 5 && abs(startY - endY) < 5) {
		return false;
	}
	return true;
}

void MouseInput::start(int startX, int startY) {
	this->startX = startX;
	this->startY = startY;
	this->endX = startX;
	this->endY = startY;
	started = true;
}

void MouseInput::reset() {
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

void MouseInput::update(int relX, int relY) {
	if (started) {
		endX += relX;
		endY += relY;
	}
}

Input::Input(int inputID, int numFollow, bool fromMouse) {
	this->inputID = inputID;
	this->numFollow = numFollow;
	this->fromMouse = fromMouse;
}

InputWrapper InputManager::convertToActions(std::vector<SDL_Event> & events) {
	InputWrapper wrapper;
	mouseInput.pressed = false;
	for (SDL_Event & sdlEvent : events) {
		switch (sdlEvent.type){
			case SDL_KEYDOWN:
				wrapper.inputs.push_back(Input{sdlEvent.key.keysym.sym, 0, false});
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					mouseInput.start(sdlEvent.motion.x, sdlEvent.motion.y);
					if (!mouseInput.left) {
						mouseInput.pressLeft(sdlEvent.motion.x, sdlEvent.motion.y);
						mouseInput.pressed = true;
					}
				}
				else if (sdlEvent.button.button == SDL_BUTTON_RIGHT){
					if (!mouseInput.right) {
						mouseInput.pressRight(sdlEvent.motion.x, sdlEvent.motion.y);
						mouseInput.pressed = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (sdlEvent.button.button = SDL_BUTTON_LEFT) {
					if (mouseInput.started) { // pushing it one last time before reset
						Input a{DRAG, 5, true};
						a.ranges.push_back(mouseInput.startX);
						a.ranges.push_back(mouseInput.startY);
						a.ranges.push_back(mouseInput.endX);
						a.ranges.push_back(mouseInput.endY);
						a.ranges.push_back(true);
						wrapper.inputs.push_back(a);
					}
					mouseInput.reset();
				}
				break;
			case SDL_MOUSEMOTION:
				if (sdlEvent.motion.state & SDL_BUTTON_LMASK) {
					mouseInput.update(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
				}
				break;
		}
	}
	if (mouseInput.isDrag()) { // DRAG, startX, startY, endX, endY, finished (0 or 1)
		Input a{ DRAG, 5, true };
		a.ranges.push_back(mouseInput.startX);
		a.ranges.push_back(mouseInput.startY);
		a.ranges.push_back(mouseInput.endX);
		a.ranges.push_back(mouseInput.endY);
		a.ranges.push_back(mouseInput.finished);
		wrapper.inputs.push_back(a);
	}
	else if (mouseInput.pressed) { // LEFT or RIGHT, x, y
		if (mouseInput.left) {
			Input a{ LEFT_PRESS, 2, true };
			a.ranges.push_back(mouseInput.clickCoordX);
			a.ranges.push_back(mouseInput.clickCoordY);
			wrapper.inputs.push_back(a);
		}
		else if (mouseInput.right) {
			Input a{ RIGHT_PRESS, 2, true };
			a.ranges.push_back(mouseInput.clickCoordX);
			a.ranges.push_back(mouseInput.clickCoordY);
			wrapper.inputs.push_back(a);
		}
	}
	int posX, posY;
	SDL_GetMouseState(&posX, &posY);
	Input a{ CURRENT_POS, 2, true };
	a.ranges.push_back(posX);
	a.ranges.push_back(posY);
	wrapper.inputs.push_back(a);
	return wrapper;
}

