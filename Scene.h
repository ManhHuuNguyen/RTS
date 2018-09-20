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
#include "ID.h"

class Scene: public CallbackInterface {

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
		std::set<Entity *> chosenOnes;
		Camera * camera;

		Scene(Camera * camera);
		void addEntity(Entity * entity);
		void addTerrainTile(TerrainTile & t);
		void loadDataToUBO();
		void update(long long elapsedMilliseconds);
		void render(long long elapsedMilliseconds);
		IntersectionRecord mousePick(Ray & ray); // this will have to be rewritten completely when I add terrain 
		IntersectionRecord mouseIntersectTerrain(Ray & r); // this will have to be rewritten completely when I add terrain
		std::vector<Entity *> dragSelect(float startX, float startY, int endX, int endY);
		void loadFogToShader(unsigned int PROGRAM, glm::vec4 fogColor = glm::vec4(1.0), float gradient = 0.0f, float density = 0.0f);
		void handleEvents(std::vector<Action> & actions) override;
		~Scene();
};

