#include "Mouse.h"

Mouse::Mouse(SDL_Window * window) {
	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	SDL_SetCursor(cursor);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WarpMouseInWindow(window, CONSTANT::WIDTH_DISPLAY / 2, CONSTANT::HEIGHT_DISPLAY / 2);
}


Ray Mouse::getMouseRay(int x, int y, Camera * camera) { // just multiplying the ray with the inverse of matrices to get back to world coord
	float normalizedDeviceX = 2.0f * x / CONSTANT::WIDTH_DISPLAY - 1.0f;
	float normalizedDeviceY = 1.0f - 2.0f * y / CONSTANT::HEIGHT_DISPLAY;
	glm::vec4 rayClip = glm::vec4(normalizedDeviceX, normalizedDeviceY, -1.0f, 1.0f); 
	glm::vec4 rayEye = CONSTANT::INV_PROJECTION_MATRIX * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
	glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(camera->getViewMatrix()) * rayEye));
	return Ray{glm::vec3(camera->eyePosition), rayWorld};
}

Mouse::~Mouse() {
	SDL_FreeCursor(cursor);
}