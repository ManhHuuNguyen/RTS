#pragma once
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"
#include <fstream>
#include <iostream>
#include "Texture.h"
#include "CONSTANTS.h"
#include "Shader.h"

class TerrainRenderer {
	// this will be rendered using indices, because it is less complex than models
	public:
		
		static const int SIZE;
		static const int VERTEX_COUNT;
		Texture * texture;
		Shader terrainShader;

		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> tCoords;
		std::vector<unsigned int> indices; 
		GLuint vao;
		GLuint vbo[3];

		TerrainRenderer();
		void generateMeshData();
		~TerrainRenderer();
		void loadTerrainToVAO();
		void render();
		void useTerrainShader();
};