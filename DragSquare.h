#pragma once
#include "GUIRenderer.h"
#include "ID.h"
#include "Ray.h"

class DragSquare : public GUI { // to be used as drag select
	
	public:
		glm::vec2 startPointInWorld = glm::vec2(-1.0f, -1.0f);
		glm::ivec2 oldStartPoint = glm::ivec2(-1, -1);
		glm::mat4 cameratMatAtStart;
		glm::vec4 cameraPosAtStart;
		bool begin = false;

		DragSquare(Texture * texture);
		void update(InputWrapper & inputWrapper, Camera * cam);
		void start(int startX, int startY, Camera * cam, float spWorldX, float spWorldY);
		glm::ivec2 computeStartPointPixel(glm::mat4 & viewMatrix);
};