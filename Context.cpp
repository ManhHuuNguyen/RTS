#include "Context.h"

Context::Context() {
	active = false;
}
Context::Context(bool active) {
	this->active = active;
}

void Context::registerCallback(CallbackInterface * callback) {
	callbacks.push_back(callback);
}

void Context::callBack(InputWrapper & inputWrapper) {
	for (int i = 0; i < callbacks.size(); i++) {
		callbacks[i]->handleEvents(inputWrapper);
	}
}