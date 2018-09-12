#include "UBO.h"

glWrapper::UBO::UBO() {
	std::cout << "UBO default constructor is called" << std::endl;
}

glWrapper::UBO::UBO(const UBO & other) {
	//std::cout << "UBO copy constructor is called" << std::endl;
	this->uboIndex = other.uboIndex;
	this->componentOffsetMap = other.componentOffsetMap;
	this->blockSize = other.blockSize;
	this->PROGRAM_HANDLER = other.PROGRAM_HANDLER;
	this->blockBuffer = new GLubyte[this->blockSize];
	memcpy(this->blockBuffer, other.blockBuffer, this->blockSize);
	isOriginal = false;
}

glWrapper::UBO::UBO(unsigned int PROGRAM_HANDLER, const char * name, const char * components[], int componentSize, int bindingPort) {
	//std::cout << "UBO constructor is called" << std::endl;
	this->PROGRAM_HANDLER = PROGRAM_HANDLER;
	GLuint blockIndex = glGetUniformBlockIndex(PROGRAM_HANDLER, name);
	std::cout << "Block index inside constructor " << blockIndex << std::endl;
	glUniformBlockBinding(PROGRAM_HANDLER, blockIndex, bindingPort);
	glGetActiveUniformBlockiv(PROGRAM_HANDLER, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	blockBuffer = new GLubyte[blockSize];
	GLuint * blockIndices = new GLuint[componentSize];
	glGetUniformIndices(PROGRAM_HANDLER, componentSize, components, blockIndices);
	
	GLint * blockOffsets = new GLint[componentSize];
	glGetActiveUniformsiv(PROGRAM_HANDLER, componentSize, blockIndices, GL_UNIFORM_OFFSET, blockOffsets);

	for (int i = 0; i < componentSize; i++) {
		componentOffsetMap[components[i]] = blockOffsets[i];
	}
	
	glGenBuffers(1, &uboIndex);
	glBindBuffer(GL_UNIFORM_BUFFER, uboIndex); // probably dont need this
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPort, uboIndex);

	delete[] blockIndices;
	delete[] blockOffsets;

	isOriginal = true;
}

void glWrapper::UBO::sendFirstTimeDataToUBO(GLuint usage) {
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, usage);
	
}

void glWrapper::UBO::sendUpdatedDataToUBO() {
	glBufferSubData(GL_UNIFORM_BUFFER, 0, blockSize, blockBuffer);
}

glWrapper::UBO::~UBO() {
	//std::cout << "UBO destructor is called. Original: " << isOriginal << std::endl;
	if (blockBuffer != nullptr) {
		delete[] blockBuffer;
	}
}