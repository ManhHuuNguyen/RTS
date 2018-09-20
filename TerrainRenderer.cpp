#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer():terrainShader("shaders/TerrainVertexShader2.vert", "shaders/TerrainFragmentShader2.frag") {
}

void TerrainRenderer::addTerrainTile(TerrainTile & t) {
	Terrain * te = t.terrain;
	terrains[te].push_back(t);
}

void TerrainRenderer::render() {
	for (auto & terrainTerrainTile : terrains) {
		std::vector<TerrainTile> & allTerrainTiles = terrainTerrainTile.second;
		std::vector<Texture *> & allTextures = terrainTerrainTile.first->textures;
		if (allTerrainTiles.size() > 0) {
			glBindVertexArray(terrainTerrainTile.first->vao);
			glUniform1ui(glGetUniformLocation(terrainShader.PROGRAM, "numTexture"), allTextures.size());
			glUniform1uiv(glGetUniformLocation(terrainShader.PROGRAM, "texturesOn"), 4, terrainTerrainTile.first->texturesOn);
			for (int i = 0; i < allTextures.size(); i++) {
				Texture * currentTexture = allTextures[i];
				glActiveTexture(GL_TEXTURE0 + currentTexture->bindingPort);
				glBindTexture(GL_TEXTURE_2D, currentTexture->texID);
				std::string samplerName = (std::string{ "texture" } +std::to_string(currentTexture->bindingPort));
				const char * textureName = samplerName.c_str();
				glUniform1i(glGetUniformLocation(terrainShader.PROGRAM, textureName), currentTexture->bindingPort);
			}
			if (terrainTerrainTile.first->blendMap != nullptr) {
				Texture * blendMap = terrainTerrainTile.first->blendMap;
				glActiveTexture(GL_TEXTURE0 + blendMap->bindingPort);
				glBindTexture(GL_TEXTURE_2D, blendMap->texID);
				glUniform1i(glGetUniformLocation(terrainShader.PROGRAM, "blendMap"), blendMap->bindingPort);
			}
			for (int i = 0; i < allTerrainTiles.size(); i++) {
				allTerrainTiles[i].draw(terrainShader.PROGRAM);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void TerrainRenderer::useTerrainShader() {
	glUseProgram(terrainShader.PROGRAM);
}
