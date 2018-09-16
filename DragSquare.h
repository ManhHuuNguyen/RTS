#pragma once
#include "GUIRenderer.h"
#include "ID.h"

class DragSquare : public GUI { // to be used as drag select
	
	public:
		// inbetween are just testing variables
		glm::vec2 startPoint1;
		glm::vec2 startPoint2;
		bool begin = false;
		// inbetween are just testing variables
		static int ID;
		DragSquare(Texture * texture);
		void handleEvents(InputWrapper & inputWrapper) override;
		void start();
};