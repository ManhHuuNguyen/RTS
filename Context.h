#pragma once

#include "CallbackInterface.h"
#include "pugixml.hpp"

class Context {

	public:
		bool active;
		Context();
		Context(bool active);
		std::vector<CallbackInterface *> callbacks;
		void registerCallback(CallbackInterface * callback);
		virtual void callBack(InputWrapper & inputWrapper);

};

struct InputMapObj {
	bool isChar;
	char charKey;
	int intKey;
	int value;
	int numFollow;
};