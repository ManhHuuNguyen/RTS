#pragma once
#include <vector>
#include "SDL/SDL.h"
#include <iostream>


class MouseInput {
	public:
		// variable for drag select
		int startX = -1;
		int startY = -1;
		int endX = -1;
		int endY = -1;
		bool finished = false;
		bool started = false;

		// variable for normal mouse click
		bool left = false;
		bool right = false;
		bool pressed = false;
		int clickCoordX = -1;
		int clickCoordY = -1;

		void start(int startX, int startY);
		void update(int relX, int relY);
		bool isDrag();
		void resetDrag();
		void resetClick();
		void pressLeft(int x, int y);
		void pressRight(int x, int y);
};

class Input {
	public:
		std::string inputID;
		std::vector<int> ranges;
		bool fromMouse;

		Input(const char * inputID, bool fromMouse);
		Input(std::string & inputID, bool fromMouse);
};

class InputWrapper {
	public:
		std::vector<Input> inputs;
};

class InputManager {

	public:		
		static std::string DRAG;
		static std::string LEFT_PRESS;
		static std::string RIGHT_PRESS;
		static std::string CURRENT_POS;
		static std::string LEFT_UP;
		static std::string RIGHT_UP;
		MouseInput mouseInput;

		InputWrapper convertToActions(std::vector<SDL_Event> & events);
};

