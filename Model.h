#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "Util.h"
#include "Animation.h"
#include "Joint.h"
#include "Mesh.h"
#include "Material.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include "CONSTANTS.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class ModelData { // is used to make health bar appear above units, and some other effects
	
	public:
		float minX = 10000.0f;
		float maxX = -10000.0f;
		float minY = 10000.0f;
		float maxY = -10000.0f;
		float minZ = 10000.0f;
		float maxZ = -10000.0f;

		float calculateHeight();
		float calculateRadius(); // radius of base circle, which is either maxX-minX or maxZ-minZ, depending on which is greater
		glm::vec2 calculateBaseCenter(); // center of base circle
		glm::vec3 calculateModelCenter();
};

class Model {

	public:
		std::vector<Material *> materials;
		Joint rootJoint;
		std::set<std::string> animatedJoints;
		std::map <std::string, Animation> animationMap;
		ModelData modelData;

		unsigned int animated = 0u; // default not animated
		bool leftHanded; // true if the model's texture is in directX specification, false if in openGL, to be used to calculate correct texture coordinate 
	
		GLuint vao;
		GLuint vbo[5];
		
		Model(const char * file, Assimp::Importer & importer, bool leftHanded, glm::vec3 scale);
		void loadModelToVAO();
		bool isTextureLoaded(std::string path, std::map<std::string, Texture *> & loadedTextures);
		std::vector<glm::vec4> getMeshesPositions();
		std::vector<glm::vec4> getMeshesNormals();
		std::vector<glm::vec2> getMeshesTextures();
		std::vector<glm::ivec4> getMeshesJoints();
		std::vector<glm::vec4> getMeshesWeights();
		
		~Model();

	private:
		void createMesh(
			const aiScene * scene,
			aiNode * rootNode,
			std::map<unsigned int, Material *> & materialMap,
			std::map<std::string, Texture *> & loadedTextures,
			glm::mat4 & parentMatrix,
			std::map<std::string, int> & jointIndexMap, 
			glm::vec4 & scale
	);
		void fillJointHierarchy(aiNode * rootNode, glm::mat4 & parentMatrix, std::map<std::string, int> & jointIndexMap);
		void fillJointHierarchyRecursive(aiNode * currentNode, Joint & parentJoint, std::map<std::string, int> & jointIndexMap);
		void calculateAnimation(const aiScene * scene);
		void normalizeVertexWeight(Mesh & mesh);
};


