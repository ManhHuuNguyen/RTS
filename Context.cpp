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