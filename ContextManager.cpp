#include "ContextManager.h"

void ContextManager::addContext(Context * context) {
	contexts.push_back(context);
}

void ContextManager::dispatch(InputWrapper & inputWrapper) {
	for (int i = 0; i < contexts.size(); i++) {
		if (contexts[i]->active) {
			contexts[i]->callBack(inputWrapper);
		}
	}
}