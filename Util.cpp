#include "Util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char * Util::loadImage(const char * filePath, int * width, int * height, int * numChannel) {
	unsigned char * imageData = stbi_load(filePath, width, height, numChannel, 0);
	return imageData;
}

void Util::freeImageData(unsigned char * imageData) {
	STBI_FREE(imageData);
}

std::pair<GLint, GLint> Util::getGLColorFormat(std::string imgType, int numChannel) {
	if (imgType == "png") {
		if (numChannel == 3) {
			return std::pair<GLint, GLint>(GL_RGB, GL_RGB);
		}
		else {
			return std::pair<GLint, GLint>(GL_RGBA, GL_RGBA);
		}
		
	}
	if (imgType == "jpg") {
		return std::pair<GLint, GLint>(GL_RGB, GL_RGB);
	}
	/*if (imgType == "bmp") {
		return std::pair<GLint, GLint>(GL_RGB, GL_BGR);
	}*/
}

int Util::getPixelSize(std::string imgType) {
	if (imgType == "png") {
		return 4;
	}
	if (imgType == "jpg") {
		return 3;
	}
	if (imgType == "bmp") {
		return 3;
	}
}


std::vector<std::string> Util::splitStringWithDelimiter(std::string & str, char c) {
	std::vector<std::string> vect;
	std::string substring;
	for (char & character : str) {
		if (character == c || character == ' ') {
			if (substring != "") {
				vect.push_back(substring);
				substring.clear();
			}
		}
		else {
			substring += character;
		}
	}
	if (substring != "") {
		vect.push_back(substring);
	}
	return vect;
}

void Util::_check_gl_error() {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cout << "GL_" << error.c_str() << std::endl;
		err = glGetError();
	}
}
void Util::printGLM(glm::mat4 matrix) {
	std::cout << matrix[0].x << " " << matrix[1].x << " " << matrix[2].x << " " << matrix[3].x << std::endl;
	std::cout << matrix[0].y << " " << matrix[1].y << " " << matrix[2].y << " " << matrix[3].y << std::endl;
	std::cout << matrix[0].z << " " << matrix[1].z << " " << matrix[2].z << " " << matrix[3].z << std::endl;
	std::cout << matrix[0].w << " " << matrix[1].w << " " << matrix[2].w << " " << matrix[3].w << std::endl;
}

void Util::printGLM(glm::vec4 vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
}
void Util::printGLM(glm::vec3 vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void Util::printGLM(glm::vec2 vec) {
	std::cout << vec.x << " " << vec.y << std::endl;
}

void Util::printGLM(glm::quat q) {
	std::cout << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
}

glm::mat4 Util::aiToGLM(aiMatrix4x4 & matrix) {
	return glm::mat4(
		matrix.a1, matrix.b1, matrix.c1, matrix.d1,
		matrix.a2, matrix.b2, matrix.c2, matrix.d2,
		matrix.a3, matrix.b3, matrix.c3, matrix.d3,
		matrix.a4, matrix.b4, matrix.c4, matrix.d4
	);
}

glm::vec3 Util::aiToGLM(aiVector3D & v) {
	return glm::vec3(v.x, v.y, v.z);
}

glm::quat Util::aiToGLM(aiQuaternion & q) {
	return glm::quat(q.w, q.x, q.y, q.z);
}

glm::vec3 Util::aiToGLM(aiColor3D & c) {
	return glm::vec3(c.r, c.g, c.b);
}

glm::vec4 Util::aiToGLM(aiColor4D & c) {
	return glm::vec4(c.r, c.g, c.b, c.a);
}

std::string Util::getCurrentFolderPath(const char * path) {
	std::string filePath{ path };
	std::size_t found = filePath.find_last_of("/\\");
	if (found != std::string::npos) {
		return filePath.substr(0, found);
	}
	return filePath;
}

std::string Util::getFileExtension(const char * path) {
	std::string filePath{ path };
	std::size_t found = filePath.find_last_of(".");
	if (found != std::string::npos) {
		return filePath.substr(found+1, filePath.length());
	}
	return filePath;
}

std::string Util::getFileExtension(std::string filePath) {
	std::size_t found = filePath.find_last_of(".");
	if (found != std::string::npos) {
		return filePath.substr(found+1, filePath.length());
	}
	return filePath;

}

std::string Util::getFileName(const char * path) {
	std::string filePath{ path };
	std::size_t found = filePath.find_last_of("/\\");
	if (found != std::string::npos) {
		return filePath.substr(found+1, filePath.length());
	}
	return filePath;
}

std::string Util::getFileNameOnly(std::string filePath) {
	std::size_t found = filePath.find_last_of(".");
	if (found != std::string::npos) {
		return filePath.substr(0, found);
	}
	return filePath;

}

void Util::printBoundingBox(BoundingBox & box) {
	Util::printGLM(box.minDimension);
	Util::printGLM(box.maxDimension);
}

bool Util::nearZero(float num) {
	return abs(num) < CONSTANT::kEpsilon;
}

float Util::normalizeRad(float rads) {
	int mod = (int)(rads / (2 * CONSTANT::MT_PI));
	rads -= 2 * CONSTANT::MT_PI * mod;
	if (rads < 0.0f) {
		rads += 2 * CONSTANT::MT_PI;
	}
	return rads;
}

float Util::normalizeDegrees(float degrees) {
	int mod = (int)(degrees / 360);
	degrees -= 360 * mod;
	if (degrees < 0) {
		degrees += 360;
	}
	return degrees;
}

float Util::rad2deg(float rads) {
	return normalizeDegrees(rads * 180.0f / CONSTANT::MT_PI);
}

float Util::deg2rad(float degs) {
	return normalizeRad(degs * CONSTANT::MT_PI / 180.0f);
}
