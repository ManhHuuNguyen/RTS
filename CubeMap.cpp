#include "CubeMap.h"

CubeMap::CubeMap(std::vector<std::string> & images, unsigned int bindingPort) {
	glGenTextures(1, &texID);
	this->bindingPort = bindingPort;
	glActiveTexture(GL_TEXTURE0 + bindingPort);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	int width, height, numChannel;
	unsigned char * imageData;
	for (GLuint i = 0; i < images.size(); i++){
		std::cout << images[i] << std::endl;
		std::string fileExtension = Util::getFileExtension(images[i]);
		imageData = Util::loadImage(images[i].c_str(), &width, &height, &numChannel);
		std::pair<GLint, GLint> textureFormat = Util::getGLColorFormat(fileExtension, numChannel);
		std::cout << numChannel << std::endl;
		if (textureFormat.first == GL_RGBA) {
			std::cout << "DAmn right" << std::endl;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat.first, width, height, 0, textureFormat.second, GL_UNSIGNED_BYTE, imageData);
		Util::freeImageData(imageData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap() {
	std::cout << "Cubemap destructor is called" << std::endl;
	glDeleteTextures(1, &texID);
}