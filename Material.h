#pragma once
#include "glm\glm.hpp"
#include "Texture.h"
#include <vector>
#include "Mesh.h"

class Material {

	public:
		glm::vec3 KA;
		glm::vec3 KD;
		glm::vec3 KS;
		Texture * diffuseTexture = nullptr;
		Texture * ambientTexture = nullptr;
		Texture * specularTexture = nullptr;
		Mesh mesh;

		Material(glm::vec3 & KD, glm::vec3 & KA, glm::vec3 & KS);
		~Material();
};