#pragma once
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"
#include <fstream>
#include <iostream>
#include "Texture.h"
#include "CONSTANTS.h"

class Terrain {
	// this will be rendered using indices, because it is less complex than models
	public:
		
		static const int SIZE;
		static const int MAX_VERTEX_COUNT;
		
		float MAX_HEIGHT;
		int VERTEX_COUNT;
		float ** heightArray;
		Texture * blendMap = nullptr;
		std::vector<Texture *> textures;
		unsigned int texturesOn[4] = {0, 0, 0, 0}; // how many texture this one has
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> tCoords;
		std::vector<unsigned int> indices; 
		GLuint vao;
		GLuint vbo[4];

		Terrain(const char * heightMapFile, const char * terrainTexture, float maxHeight);
		Terrain(const char * heightMapFile, const char * blendMap, const char ** terrainTextures, unsigned int numTextures, float maxHeight);
		void generateMeshData(const char * heightMapFile, float maxHeight);
		~Terrain();
		void loadTerrainToVAO();

	private:
		float calculateVertexHeight(int x, int z, int width, int height, int numChannel, unsigned char * imageData);
};