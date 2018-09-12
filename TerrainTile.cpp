#include "TerrainTile.h"


void TerrainTile::draw(unsigned int PROGRAM_HANDLER) {
	glm::mat4 modelMat = glm::translate(glm::vec3(x*Terrain::SIZE, 0.0f, z*Terrain::SIZE));
	glUniformMatrix4fv(glGetUniformLocation(PROGRAM_HANDLER, "modelMat"), 1, GL_FALSE, value_ptr(modelMat));
	glDrawElements(GL_TRIANGLES, terrain->indices.size(), GL_UNSIGNED_INT, (void*)0);
}

TerrainTile::TerrainTile(int x, int z, Terrain * terrain) {
	this->x = x;
	this->z = z;
	this->terrain = terrain;
}