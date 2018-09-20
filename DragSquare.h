#pragma once
#include "GUIRenderer.h"
#include "ID.h"
#include "Ray.h"

class DragSquare : public GUI { // to be used as drag select
	
	public:
		// inbetween are just testing variables
		glm::vec2 startPoint = glm::vec2(-1.0f, -1.0f);
		glm::vec2 startPointInPixel = glm::vec2(-1.0f, -1.0f);
		bool begin = false;
		// inbetween are just testing variables
		DragSquare(Texture * texture);
		void update(InputWrapper & inputWrapper, Camera * cam);
		void start(float startX, float startY);
		void computeStartPointPixel(glm::mat4 viewMatrix);
};