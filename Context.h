#pragma once

#include "CallbackInterface.h"


class Context {

	public:
		bool active;
		Context();
		Context(bool active);
		std::vector<CallbackInterface *> callbacks;
		void registerCallback(CallbackInterface * callback);
		void callBack(InputWrapper & inputWrapper);

};



