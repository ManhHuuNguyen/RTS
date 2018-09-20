#pragma once
#include "GUI.h"
#include "CallbackInterface.h"

class ActiveGUI :public GUI, public CallbackInterface { // this will be buttons and such
	public:
		ActiveGUI(Texture * texture, int startX, int startY, int endX, int endY, int id);
		virtual void handleEvents(std::vector<Action> & actions) = 0;
};
