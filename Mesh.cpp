#include "Mesh.h"

std::vector<glm::vec4> Mesh::getPositions() {
	std::vector<glm::vec4> positions;
	for (int i = 0; i < vertices.size(); i++) {
		positions.push_back(vertices[i].position);
	}
	return positions;
}
std::vector<glm::vec4> Mesh::getNormals() {
	std::vector<glm::vec4> normals;
	for (int i = 0; i < vertices.size(); i++) {
		normals.push_back(vertices[i].normal);
	}
	return normals;

}
std::vector<glm::vec2> Mesh::getTextures() {
	std::vector<glm::vec2> textures;
	for (int i = 0; i < vertices.size(); i++) {
		textures.push_back(vertices[i].textureCoord);
	}
	return textures;

}
std::vector<glm::ivec4> Mesh::getJoints() {
	std::vector<glm::ivec4> joints;
	for (int i = 0; i < vertices.size(); i++) {
		joints.push_back(vertices[i].jointIDs);
	}
	return joints;
}
std::vector<glm::vec4> Mesh::getWeights() {
	std::vector<glm::vec4> weights;
	for (int i = 0; i < vertices.size(); i++) {
		weights.push_back(vertices[i].weights);
	}
	return weights;

}