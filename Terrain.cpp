#include "Terrain.h"


const int Terrain::SIZE = 800;
const int Terrain::MAX_VERTEX_COUNT = 256;

void Terrain::generateMeshData(const char * heightMapFile, float maxHeight) {
	MAX_HEIGHT = maxHeight;
	int numChannel;
	int width;
	int height;
	unsigned char * imageData = Util::loadImage(heightMapFile, &width, &height, &numChannel); // um, would we reload multiple images because of this?
	VERTEX_COUNT = std::min(std::min(width, height), MAX_VERTEX_COUNT); // though width and height should always be equal
	float tileSize = (float)SIZE / (VERTEX_COUNT - 1);
	heightArray = new float*[VERTEX_COUNT];
	for (int s = 0; s < VERTEX_COUNT; s++) {
		heightArray[s] = new float[VERTEX_COUNT];
	}

	vertices.reserve(VERTEX_COUNT * VERTEX_COUNT);
	normals.reserve(VERTEX_COUNT * VERTEX_COUNT);
	tCoords.reserve(VERTEX_COUNT * VERTEX_COUNT);
	indices.reserve(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			float height1 = calculateVertexHeight(x, z, width, height, numChannel, imageData);
			heightArray[z][x] = height1;
			vertices.push_back(glm::vec3(x*tileSize, height1, z*tileSize));
			tCoords.push_back(glm::vec2((float)x / (VERTEX_COUNT - 1), (float)z / (VERTEX_COUNT - 1)));
		}
	}

	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) { // weird, fast way to compute normals https://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid
			float heightLeft = (x - 1 >= 0) ? heightArray[z][x - 1] : 0.0f;
			float heightRight = (x + 1 < VERTEX_COUNT) ? heightArray[z][x + 1] : 0.0f;
			float heightDown = (z - 1 >= 0) ? heightArray[z - 1][x] : 0.0f;
			float heightUp = (z + 1 < VERTEX_COUNT) ? heightArray[z + 1][x] : 0.0f;
			normals.push_back(glm::normalize(glm::vec3(heightLeft - heightRight, 2.0f, heightDown-heightUp)));
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

	// freeing resources
	Util::freeImageData(imageData);
	loadTerrainToVAO();
}

void Terrain::loadTerrainToVAO() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(4, vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tCoords.size() * sizeof(glm::vec2), &tCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

Terrain::Terrain(const char * heightMapFile, const char * blendMapFile, const char ** terrainTextures, unsigned int numTextures, float maxHeight) {
	std::ifstream heightMap(heightMapFile);
	if (!heightMap.good()) {
		std::cout << "Cannot load " << heightMapFile << std::endl;
		return;
	}
	std::ifstream blendMapF(blendMapFile);
	if (!blendMapF.good()) {
		std::cout << "Cannot load " << blendMapFile << std::endl;
		return;
	}
	for (int i = 0; i < numTextures; i++) {
		std::ifstream textureMap(terrainTextures[i]);
		if (!textureMap.good()) {
			std::cout << "Cannot load " << terrainTextures[i] << std::endl;
			return;
		}
	}
	blendMap = new Texture{ blendMapFile, numTextures, GL_MIRRORED_REPEAT };

	for (unsigned int i = 0u; i < numTextures; i++) {
		Texture * t = new Texture{terrainTextures[i], i, GL_MIRRORED_REPEAT};
		textures.push_back(t);
		texturesOn[i] = 1u;
	}
	generateMeshData(heightMapFile, maxHeight);
}

Terrain::Terrain(const char * heightMapFile, const char * terrainTexture, float maxHeight) {
	
	std::ifstream heightMap(heightMapFile);
	if (!heightMap.good()) {
		std::cout << "Cannot load " << heightMapFile << std::endl;
		return;
	}

	std::ifstream terrainF(terrainTexture);
	if (!terrainF.good()) {
		std::cout << "Cannot load " << terrainTexture << std::endl;
		return;
	}
	Texture * t = new Texture{terrainTexture, 0, GL_MIRRORED_REPEAT}; // um, would we reload multiple images because of this?
	texturesOn[0] = 1u;
	textures.push_back(t);
	// load height map
	generateMeshData(heightMapFile, maxHeight);
}

/**
1) Read the heightmap value;
2) Scale the heightmap to [-1,1] : h = 2*h - 1.0;
3) Scale by some real world amount h = wscale*h;
**/
float Terrain::calculateVertexHeight(int x, int z, int width, int height, int numChannel, unsigned char * imageData) {
	int pixelW = static_cast<int>(x / (float)(VERTEX_COUNT-1) * (float)(width-1));
	int pixelH = static_cast<int>(z / (float)(VERTEX_COUNT-1) * (float)(height-1)); // this is wrong, it will get to 256 if z or x = 255 and width, height = 256
	int pixelR = (pixelH * width + pixelW) * numChannel;
	float heightVal = 0.0f;
	heightVal += ((unsigned int)imageData[pixelR])* 0.2989f; // to calculate grey scale value
	heightVal += ((unsigned int)imageData[pixelR + 1]) * 0.587f;
	heightVal += ((unsigned int)imageData[pixelR + 2]) * 0.114f;
	return heightVal / 256.0f * MAX_HEIGHT;
}

Terrain::~Terrain() {
	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
	if (blendMap != nullptr) {
		delete blendMap;
	}
	for (int s = 0; s < VERTEX_COUNT; s++) {
		delete[] heightArray[s];
	}
	delete[] heightArray;
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(4, vbo);
}
