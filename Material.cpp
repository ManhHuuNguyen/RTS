#include "Material.h"

Material::Material(glm::vec3 & KD, glm::vec3 & KA, glm::vec3 & KS) {
	this->KA = KA;
	this->KD = KD;
	this->KS = KS;
}

Material::~Material() {
	if (diffuseTexture != nullptr) {
		delete diffuseTexture;
	}
	if (ambientTexture != nullptr) {
		delete ambientTexture;
	}
	if (specularTexture != nullptr) {
		delete specularTexture;
	}
}