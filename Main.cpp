
#include "GL\glew.h"
#include "SDL/SDL.h"
#include "SDL\SDL_opengl.h"
#include <map>
#include <string>
#include "Scene.h"
#include "GameClock.h"
#include <iostream>
#include <fstream>
#include "Worker.h"
#include "CONSTANTS.h"
#include "assimp\Importer.hpp"
#include "Mouse.h"
#include "InputManager.h"
#include "ContextManager.h"


void setUpGLContext();
void setUpGLMain();
void loadModels(std::map<std::string, Model *> & models, Assimp::Importer & importer);
void loadTerrains(std::map<std::string, Terrain *> & terrains);
void loadTerrainTiles(std::map<std::string, Terrain *> & terrains, Scene & scene);
void loadGUITexture(std::map<std::string, Texture *> & guis);
void freeResources(std::map<std::string, Model *> & models, std::map<std::string, Terrain *> & terrains, std::map<std::string, Texture *> & guis);


int main(int argc, char *argv[]) {
	// setting up window and context
	std::set<Entity *> chosenOnes;
	setUpGLContext();
	SDL_Window * window = SDL_CreateWindow("Diodeas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONSTANT::WIDTH_DISPLAY, CONSTANT::HEIGHT_DISPLAY, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	setUpGLMain();
	InputManager inputManager{};
	
	Scene scene{};
	Camera camera{ 0.785f, 90.0f };

	// loading contexts
	ContextManager contextManager{};
	Context mainContext{true};
	contextManager.addContext(&mainContext);
	mainContext.registerCallback(&camera);

	// resources loading
	std::map<std::string, Model *> models;
	std::map<std::string, Terrain *> terrains;
	std::map<std::string, Texture *> guis;
	Assimp::Importer importer = Assimp::Importer();
	loadGUITexture(guis);
	loadModels(models, importer);
	loadTerrains(terrains);
	loadTerrainTiles(terrains, scene);
	
	Mouse mouse{window};

	// load entities

	GUI dragSquare{guis["DRAG_SQUARE"], 0, 0, 0, 0};
	scene.guiRenderer.addGUI(&dragSquare);
	
	Worker steve1{ models["RUNNING_MODEL"], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve1);

	Worker steve4{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(10.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve4);

	Worker steve5{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(50.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve5);

	Worker steve6{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(60.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve6);

	Worker steve7{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(70.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve7);
	
	Worker steve8{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(80.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve8);
	
	Worker steve9{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(90.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve9);
	
	Worker steve10{ models["RUNNING_MODEL"], glm::vec3(0.0f, 1.57f, 0.0f), glm::vec3(100.0f, 0.0f, 20.0f), glm::vec3(1.0f, 1.0f, 1.0f) };
	scene.addEntity(&steve10);

	scene.octree->updateTree();
	
	long long totalMilliSeconds = 0;
	int totalFrameDrawn = 0;
	bool QUIT = false;
	GameClock gameClock{};
	SDL_Event sdlEvent;
	
	
	while (!QUIT) {
		std::vector<SDL_Event> sdlEvents;
		
		while (SDL_PollEvent(&sdlEvent) != 0) { // if key is pressed
			if (sdlEvent.type == SDL_QUIT) {
				QUIT = true;
			}
			else {
				sdlEvents.push_back(sdlEvent);
			}		
		}
		InputWrapper inputWrapper = inputManager.convertToActions(sdlEvents);
		
		contextManager.dispatch(inputWrapper);
		
		dragSquare.updateModelMat(inputManager.mouseAction.startX, inputManager.mouseAction.startY, inputManager.mouseAction.endX, inputManager.mouseAction.endY);
		//IntersectionRecord hitRecord = scene.mousePick(mouseRay);
		/*if (hitRecord.e_ptr != nullptr) {
			if (chosenOnes.count(hitRecord.e_ptr) != 0) {
				chosenOnes.erase(hitRecord.e_ptr);
			}
			else {
				chosenOnes.insert(hitRecord.e_ptr);
			}
				
		}
		
		
		if (chosenOnes.size() > 0 && hitRecord.e_ptr == nullptr && hitRecord.groundHitPoint.x <= CONSTANT::WORLD_SIZE) {
			for (auto pickedEntity : chosenOnes) {
				pickedEntity->moveToward(hitRecord.groundHitPoint.x, hitRecord.groundHitPoint.y);
			}
				
		}*/
		
		long long elapsedMilliSeconds = gameClock.getTimePast();
		totalMilliSeconds += elapsedMilliSeconds;
		totalFrameDrawn += 1;
		scene.update(elapsedMilliSeconds, camera);
		// draw calls
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.render(elapsedMilliSeconds, camera, chosenOnes);
		SDL_GL_SwapWindow(window);
		/*if (totalMilliSeconds / 1000.0f >= 1) {
			std::cout << "FPS: " << (float)totalFrameDrawn / totalMilliSeconds * 1000 << std::endl;
		}*/
	}
	SDL_DestroyWindow(window);
	freeResources(models, terrains, guis);
	window = NULL;
	SDL_Quit();
	return 0;
}

void setUpGLContext() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void setUpGLMain() {
	glewInit();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}

void loadModels(std::map<std::string, Model *> & models, Assimp::Importer & importer) {
	/*Model * CHICKEN_MODEL = new Model{ "resources/eagle/eagle.fbx", importer, true };
	models.insert(std::pair<std::string, Model *>("CHICKEN_MODEL", CHICKEN_MODEL));*/
	Model * RUNNING_MODEL = new Model{ "resources/running-model/running_model.dae", importer, true };
	models.insert(std::pair<std::string, Model *>("RUNNING_MODEL", RUNNING_MODEL));
	Model * STEVE = new Model{"resources/steve-obj/steve.obj", importer, true};
	models.insert(std::pair<std::string, Model *>("STEVE", STEVE));
	/*Model * TREE_MODEL = new Model{"resources/tree/Tree.fbx", importer, true};
	models.insert(std::pair<std::string, Model *>("TREE_MODEL", TREE_MODEL));*/
}

void loadGUITexture(std::map<std::string, Texture *> & guis) {
	Texture * dragSquareTex = new Texture("resources/guis/mouse_square.png", 0, GL_CLAMP_TO_EDGE);
	guis.insert(std::pair<std::string, Texture *>("DRAG_SQUARE", dragSquareTex));
}

void loadTerrains(std::map<std::string, Terrain *> & terrains) {
	const char * textureNames[] = { 
		"resources/terrain/grassy_hill/grassy2.png",
		"resources/terrain/grassy_hill/mud.png",
		"resources/terrain/grassy_hill/grassFlowers.png",
		"resources/terrain/grassy_hill/path.png"
	};
	Terrain * GRASSY_HILL = new Terrain{ "resources/terrain/grassy_hill/heightmap1.png" ,"resources/terrain/grassy_hill/blendMap.png", textureNames, 1, 230.0f };
	terrains.insert(std::pair<std::string, Terrain*>("GRASSY_HILL", GRASSY_HILL));
}

void loadTerrainTiles(std::map<std::string, Terrain *> & terrains, Scene & scene) {
	TerrainTile terrainTile1{ 0, 0, terrains["GRASSY_HILL"] };
	TerrainTile terrainTile2{ -1, 0, terrains["GRASSY_HILL"] };
	TerrainTile terrainTile3{ -1, -1, terrains["GRASSY_HILL"] };
	TerrainTile terrainTile4{ 0, -1, terrains["GRASSY_HILL"] };
	scene.addTerrainTile(terrainTile1);
	scene.addTerrainTile(terrainTile2);
	scene.addTerrainTile(terrainTile3);
	scene.addTerrainTile(terrainTile4);
}

void freeResources(std::map<std::string, Model *> & models, std::map<std::string, Terrain *> & terrains, std::map<std::string, Texture *> & guis) {
	for (auto const & model : models) {
		delete model.second;
	}
	for (auto const & terrain : terrains) {
		delete terrain.second;
	}
	for (auto const & guiTexture : guis) {
		delete guiTexture.second;
	}
}

