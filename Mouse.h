#pragma once

#include "SDL/SDL.h"
#include "Camera.h"
#include "CONSTANTS.h"
#include "Ray.h"

class Mouse {

	public:
		SDL_Cursor * cursor;

		Mouse(SDL_Window * window);
		Ray getMouseRay(int x, int y, Camera * camera);
		~Mouse();
};
