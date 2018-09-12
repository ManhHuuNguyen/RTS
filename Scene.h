#pragma once
#include "glm\glm.hpp"
#include<glm/gtc/quaternion.hpp>
#include<glm/common.hpp>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "FollowCamera.h"
#include "GameClock.h"
#include <vector>
#include "Light.h"
#include "TerrainRenderer.h"
#include "EntityRenderer.h"
#include "GUIRenderer.h"
#include "HealthBarRenderer.h"
#include "BoundingBoxRenderer.h"
#include "UBO.h"
#include "Terrain.h"
#include "Octree.h"

class Scene {

	public:
		Light ambientLight = { glm::vec4(0.4f, 0.4f, 0.4f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) };
		Light pointLight = { glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(-9687.0f, 7986.0f, 8976.0f, 0.0f) };
		EntityRenderer entityRenderer;
		TerrainRenderer terrainRenderer;
		GUIRenderer guiRenderer;
		HealthBarRenderer healthbarRenderer;
		BoundingBoxRenderer boundingboxRenderer;
		std::map<std::string, glWrapper::UBO> uniformBlocks;
		Octree * octree;

		Scene();
		void addEntity(Entity * entity);
		void addTerrainTile(TerrainTile & t);
		void loadDataToUBO();
		void update(long long elapsedMilliseconds, Camera & camera);
		void render(long long elapsedMilliseconds, Camera & camera, std::set<Entity *> & chosenOnes);
		IntersectionRecord mousePick(Ray & ray);
		void loadFogToShader(unsigned int PROGRAM, glm::vec4 fogColor = glm::vec4(1.0), float gradient = 0.0f, float density = 0.0f);
		~Scene();
};
