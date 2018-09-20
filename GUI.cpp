#include "GUI.h"


GUI::GUI(Texture * texture, int startX, int startY, int endX, int endY) {
	this->texture = texture;
	updateModelMat(startX, startY, endX, endY);
}

glm::mat4 GUI::getModelMat() {
	return glm::translate(glm::vec3(position, 0.0f)) * glm::scale(glm::vec3(scale, 1.0f));
}

void GUI::updateModelMat(int startX, int startY, int endX, int endY) {// convert screen pixel to normalized device coordinate (-1, 1)
	int width = abs(startX - endX);
	int height = abs(startY - endY);
	int halfWidthDisplay = CONSTANT::WIDTH_DISPLAY / 2;
	int halfHeightDisplay = CONSTANT::HEIGHT_DISPLAY / 2;
	float ax = (float)width / CONSTANT::WIDTH_DISPLAY;
	float ay = (float)height / CONSTANT::HEIGHT_DISPLAY;
	scale = glm::vec2(ax, ay);


	float startxNDC = Util::mapInterval((float)startX, 0.0f, (float)CONSTANT::WIDTH_DISPLAY, -1.0f, 1.0f);// where i want to get to
	float startyNDC = Util::mapInterval((float)startY, 0.0f, (float)CONSTANT::HEIGHT_DISPLAY, 1.0f, -1.0f);
	// range of startyNDC goes from 1.0f to -1.0f because direction of y axis of normalized device coordinate and screen coordinate are opposite
	float xNDC, yNDC; // where I am at
	if (startX < endX) {
		if (startY < endY) {
			xNDC = -ax;
			yNDC = ay;
		}
		else {
			xNDC = -ax;
			yNDC = -ay;
		}
	}
	else {
		if (startY < endY) {
			xNDC = ax;
			yNDC = ay;
		}
		else {
			xNDC = ax;
			yNDC = -ay;
		}
	}
	position = glm::vec2(startxNDC - xNDC, startyNDC - yNDC);
}