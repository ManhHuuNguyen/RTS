#include "TerrainRenderer.h"


const int TerrainRenderer::SIZE = 16;
const int TerrainRenderer::VERTEX_COUNT = CONSTANT::WORLD_SIZE / SIZE;

void TerrainRenderer::generateMeshData() {

	vertices.reserve(VERTEX_COUNT * VERTEX_COUNT);
	tCoords.reserve(VERTEX_COUNT * VERTEX_COUNT);
	indices.reserve(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			vertices.push_back(glm::vec2(x*SIZE-CONSTANT::WORLD_SIZE/2, z*SIZE - CONSTANT::WORLD_SIZE/2));
			tCoords.push_back(glm::vec2((float)x / (VERTEX_COUNT - 1), (float)z / (VERTEX_COUNT - 1)));
		}
	}


	for (unsigned int z = 0u; z < VERTEX_COUNT - 1u; z++) {
		for (unsigned int x = 0u; x < VERTEX_COUNT - 1u; x++) {
			unsigned int topLeft = z * VERTEX_COUNT + x;
			unsigned int topRight = topLeft + 1u;
			unsigned int bottomLeft = topLeft + VERTEX_COUNT;
			unsigned int bottomRight = bottomLeft + 1u;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	loadTerrainToVAO();
}

void TerrainRenderer::loadTerrainToVAO() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tCoords.size() * sizeof(glm::vec2), &tCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

TerrainRenderer::TerrainRenderer():terrainShader("shaders/TerrainVertexShader2.vert", "shaders/TerrainFragmentShader2.frag") {
	const char * terrainTexture = "resources/terrain/grassy_hill/red_dirt.png";
	std::ifstream terrainF(terrainTexture);
	if (!terrainF.good()) {
		std::cout << "Cannot load " << terrainTexture << std::endl;
		return;
	}
	texture = new Texture{terrainTexture, 0, GL_MIRRORED_REPEAT}; // um, would we reload multiple images because of this?
	generateMeshData();
}



TerrainRenderer::~TerrainRenderer() {
	if (texture != nullptr) {
		delete texture;
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(3, vbo);
}

void TerrainRenderer::render() {
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0 + texture->bindingPort);
	glBindTexture(GL_TEXTURE_2D, texture->texID);
	glUniform1i(glGetUniformLocation(terrainShader.PROGRAM, "texture0"), texture->bindingPort);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void TerrainRenderer::useTerrainShader() {
	glUseProgram(terrainShader.PROGRAM);
}
