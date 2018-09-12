#pragma once
#include "GL\glew.h"
#include "GL\glext.h"
#include <map>
#include <iostream>

namespace glWrapper {
	// just an interface to facilitate opengl process

	class UBO {

		public:
			GLuint uboIndex; 
			std::map<std::string, GLint> componentOffsetMap;
			GLubyte * blockBuffer = nullptr;
			GLint blockSize;
			unsigned int PROGRAM_HANDLER;
			bool isOriginal; // for testing purpose only, delete later

			UBO();
			UBO(unsigned int PROGRAM_HANDLER, const char * name, const char * components[], int componentSize, int bindingPort);
			UBO(const UBO & other);
			void sendFirstTimeDataToUBO(GLuint usage); // GL_DYNAMIC_DRAW, GL_STREAM_COPY, etc
			void sendUpdatedDataToUBO();
			~UBO();
	};

	template <class T>
	void updateUBOComponent(UBO & u, const char * componentName, T * componentData, int componentSize) {
		memcpy(u.blockBuffer + u.componentOffsetMap[componentName], componentData, componentSize);
	}
}