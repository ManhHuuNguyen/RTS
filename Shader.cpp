#include "Shader.h"

Shader::Shader(const char * vertexShaderName, const char * fragmentShaderName) {
	std::ifstream vertexShaderFile(vertexShaderName);
	if (!vertexShaderFile.good()) {
		std::cout << "Cannot load " << vertexShaderName << std::endl;
		return;
	}
	std::string shaderStr((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	GLchar* vertexShaderCode = (char *)shaderStr.c_str();
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	// check vertex shader compilation log
	GLint vertexShaderCompileLogLength = 0;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &vertexShaderCompileLogLength);
	if (vertexShaderCompileLogLength > 0) {
		GLchar * vertexShaderLog = (GLchar *)malloc(vertexShaderCompileLogLength);
		glGetShaderInfoLog(vertexShader, vertexShaderCompileLogLength, &vertexShaderCompileLogLength, vertexShaderLog);
		std::cout << "Vertex Shader compile log:" << std::endl;
		std::cout << vertexShaderLog << std::endl;
		free(vertexShaderLog);
	}

	GLint vertexShaderCompileStatus = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompileStatus);
	if (vertexShaderCompileStatus == GL_FALSE) {
		std::cout << "Cannot compile vertex shader" << std::endl;
		glDeleteShader(vertexShader);
		return;
	}

	std::ifstream fragmentShaderFile(fragmentShaderName);
	if (!fragmentShaderFile.good()) {
		std::cout << "Cannot load " << fragmentShaderName << std::endl;
		return;
	}

	std::string fragmentShaderStr((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
	GLchar* fragmentShaderCode = (char *)fragmentShaderStr.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	// check fragment shader compilation log
	GLint fragmentShaderCompileLogLength = 0;
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &fragmentShaderCompileLogLength);
	if (fragmentShaderCompileLogLength > 0) {
		GLchar * fragmentShaderLog = (GLchar *)malloc(fragmentShaderCompileLogLength);
		glGetShaderInfoLog(fragmentShader, fragmentShaderCompileLogLength, &fragmentShaderCompileLogLength, fragmentShaderLog);
		std::cout << "Fragment Shader compile log:" << std::endl;
		std::cout << fragmentShaderLog << std::endl;
		free(fragmentShaderLog);
	}

	GLint fragmentShaderCompileStatus = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompileStatus);
	if (fragmentShaderCompileStatus == GL_FALSE) {
		std::cout << "Cannot compile fragment shader" << std::endl;
		glDeleteShader(fragmentShader);
		return;
	}
	PROGRAM = glCreateProgram();
	glAttachShader(PROGRAM, vertexShader);
	glAttachShader(PROGRAM, fragmentShader);
	glLinkProgram(PROGRAM);
	// detach + delete shader after linking
	glDetachShader(PROGRAM, vertexShader);
	glDetachShader(PROGRAM, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}