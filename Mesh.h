#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "Vertex.h"

class Mesh {
	public:
		std::vector<Vertex> vertices;
	
		std::vector<glm::vec4> getPositions();
		std::vector<glm::vec4> getNormals();
		std::vector<glm::vec2> getTextures();
		std::vector<glm::ivec4> getJoints();
		std::vector<glm::vec4> getWeights();
		// will use glDrawArrays
};
