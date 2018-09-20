#pragma once
#include "InputManager.h"


class Action {
	public:
		std::string key;
		std::vector<float> floatRanges;
		std::vector<int> intRanges;
		bool fromMouse;

		Action(const char * charKey, bool fromMouse);
		Action(std::string & key, bool fromMouse);
		
};


class CallbackInterface {

	public:
		int id;
		CallbackInterface(int id);
		virtual void handleEvents(std::vector<Action> & actions) = 0;
};