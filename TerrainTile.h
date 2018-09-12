#pragma once
#include "Terrain.h"

class TerrainTile {

	public:

		int x;
		int z;
		Terrain * terrain;

		TerrainTile(int x, int z, Terrain * terrain);
		void draw(unsigned int PROGRAM_HANDLER);
};