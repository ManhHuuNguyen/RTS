#include "Texture.h"

Texture::Texture(std::string path, unsigned int bindingPort, GLuint texParam) {
	std::ifstream my_file(path);
	if (my_file.good()){
		unsigned char * imageData;
		int numChannel;
		std::string fileExtension = Util::getFileExtension(path);
		imageData = Util::loadImage(path.c_str(), &width, &height, &numChannel);
		glGenTextures(1, &texID);
		this->bindingPort = bindingPort;
		glActiveTexture(GL_TEXTURE0 + bindingPort);
		glBindTexture(GL_TEXTURE_2D, texID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		std::pair<GLint, GLint> textureFormat = Util::getGLColorFormat(fileExtension, numChannel);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.first, width, height, 0, textureFormat.second, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		
		
		glBindTexture(GL_TEXTURE_2D, 0); // bind current state to default texture. since we dont have a default one, it effectively means unbind current texture
		fileName = path;
		Util::freeImageData(imageData);
	}
	else {
		std::cout << "Cannot load " << path << std::endl;
	}
	my_file.close();
}

Texture::~Texture() {
	glDeleteTextures(1, &texID);
	std::cout << "Texture destructor is called" << std::endl;
}