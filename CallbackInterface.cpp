#include "CallbackInterface.h"

CallbackInterface::CallbackInterface(int id) {
	this->id = id;
}


Action::Action(const char * charKey, bool fromMouse) {
	this->fromMouse = fromMouse;
	this->key = charKey;
}

Action::Action(std::string & key, bool fromMouse) {
	this->fromMouse = fromMouse;
	this->key = key;
}