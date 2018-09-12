#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

#include "GL\glew.h"
#include "GL\glext.h"
#include "BoundingBox.h"
 
#include "CONSTANTS.h"
#include "assimp\scene.h"

namespace Util {
	std::vector<std::string> splitStringWithDelimiter(std::string & str, char c);

	unsigned char * loadImage(const char * filePath, int * width, int * height, int * numChannel);
	void freeImageData(unsigned char * imageData);

	void _check_gl_error();

	int getPixelSize(std::string imgType);
	std::pair<GLint, GLint> getGLColorFormat(std::string imgType, int numChannel);

	void printGLM(glm::mat4 matrix);
	void printGLM(glm::vec4 vec);
	void printGLM(glm::vec3 vec);
	void printGLM(glm::vec2 vec);
	void printGLM(glm::quat q);

	void printBoundingBox(BoundingBox & box);

	glm::mat4 aiToGLM(aiMatrix4x4 & matrix);
	glm::vec3 aiToGLM(aiVector3D & v);
	glm::quat aiToGLM(aiQuaternion & q);
	glm::vec3 aiToGLM(aiColor3D & c); // rgb value
	glm::vec4 aiToGLM(aiColor4D & c); // rgba value

	
	template <typename T>
	T mapInterval(T x, T xLow, T xHigh, T yLow, T yHigh) {
		return (x - xLow)*(yHigh - yLow) / (xHigh - xLow) + yLow;
	}

	std::string getCurrentFolderPath(const char * path);
	std::string getFileExtension(const char * path);
	std::string getFileExtension(std::string path);
	std::string getFileName(const char * path);
	std::string getFileNameOnly(std::string path);

	bool nearZero(float num);
	float normalizeRad(float rad);
	float normalizeDegrees(float degrees);
	float rad2deg(float rads);
	float deg2rad(float degs);
}
