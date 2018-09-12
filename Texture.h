#pragma once
#include "GL\glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Util.h"

class Texture {

	public:
		GLuint texID;
		unsigned int bindingPort;
		std::string fileName;
		int width;
		int height;

		Texture(std::string path, unsigned int bindPort, GLuint texParam);
		~Texture();
};
