#include "Mouse.h"

Mouse::Mouse(SDL_Window * window) {
	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	SDL_SetCursor(cursor);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WarpMouseInWindow(window, CONSTANT::WIDTH_DISPLAY / 2, CONSTANT::HEIGHT_DISPLAY / 2);
}

Mouse::~Mouse() {
	SDL_FreeCursor(cursor);
}