#pragma once

#include "Texture.h"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"

class GUI {

public:
	Texture * texture;
	glm::vec2 position; // considered as center of rectangle
	glm::vec2 scale;

	GUI(Texture * texture, int startX, int startY, int endX, int endY); // pixel on screen
	glm::mat4 getModelMat();
	void updateModelMat(int startX, int startY, int endX, int endY); // pixel on screen

};