#pragma once
#include "TerrainTile.h"
#include "Shader.h"
#include <map>

class TerrainRenderer {
public:
	std::map<Terrain *, std::vector<TerrainTile>> terrains;
	Shader terrainShader;

	TerrainRenderer();
	void addTerrainTile(TerrainTile & e);
	void render();
	void useTerrainShader();
};