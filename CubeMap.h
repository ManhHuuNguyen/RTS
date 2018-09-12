#pragma once
#include "GL\glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Util.h"

class CubeMap {

	public:
		GLuint texID;
		unsigned int bindingPort;

		CubeMap(std::vector<std::string> & images, unsigned int bindingPort);
		~CubeMap();
};