#pragma once
#include <fstream>
#include <iostream>
#include "GL\glew.h"
#include "GL\glext.h"

class Shader {

	public:
		GLuint PROGRAM;
		
		Shader(const char * vertexShaderName, const char * fragmentShaderName);
};