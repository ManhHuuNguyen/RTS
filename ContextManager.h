#pragma once
#include "Context.h"

class ContextManager {
	public:
		std::vector<Context *> contexts;
		
		void addContext(Context * context);
		void dispatch(InputWrapper & inputWrapper);
};