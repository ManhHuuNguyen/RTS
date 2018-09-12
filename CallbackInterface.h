#pragma once
#include "InputManager.h"
class CallbackInterface {


	public:
		virtual void handleEvents(InputWrapper & inputWrapper) = 0;
};