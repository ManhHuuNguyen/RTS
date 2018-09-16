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
		void reset();
		void pressLeft(int x, int y);
		void pressRight(int x, int y);
};

class Input {
	public:
		int inputID;
		std::vector<int> ranges;
		std::vector<float> floatRanges;
		int numFollow;
		bool fromMouse;

		Input(int inputID, int numFollow, bool fromMouse);
};

class InputWrapper {
	public:
		std::vector<Input> inputs;
};

class InputManager {

	public:		
		static int DRAG;
		static int LEFT_PRESS;
		static int RIGHT_PRESS;
		static int CURRENT_POS;
		static int LEFT_UP;
		static int RIGHT_UP;
		MouseInput mouseInput;

		InputWrapper convertToActions(std::vector<SDL_Event> & events);
};

