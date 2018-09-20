#include "Scene.h"


Scene::~Scene() {
	delete octree;
}

Scene::Scene(Camera * camera) :terrainRenderer(), entityRenderer(), guiRenderer(), healthbarRenderer(), boundingboxRenderer(), CallbackInterface(ID::SCENE_ID) {
	//loadDataToUBO();
	glm::vec3 minDimension = glm::vec3(-CONSTANT::WORLD_SIZE / 2.0f);
	glm::vec3 maxDimension = glm::vec3(CONSTANT::WORLD_SIZE / 2.0f);
	BoundingBox worldBound{minDimension, maxDimension};
	octree = new Octree{worldBound};
	this->camera = camera;
}

void Scene::addEntity(Entity * entity) {
	octree->addEntityToQueue(entity);
	// add entity whose bounding box needs to be rendered to boundingboxRenderer
	boundingboxRenderer.boxOfEntities.push_back(entity);
}
void Scene::addTerrainTile(TerrainTile & t) {
	terrainRenderer.addTerrainTile(t);
}

void Scene::loadDataToUBO() {
	// just passing in dummy value to initialize
	GLuint PROGRAM_HANDLER = entityRenderer.entityShader.PROGRAM;
	GLuint blockIndex = glGetUniformBlockIndex(PROGRAM_HANDLER, "lightProperty");
	std::cout << "Block Index " << blockIndex << std::endl;
	blockIndex = glGetUniformBlockIndex(PROGRAM_HANDLER, "MatrixBlock");
	std::cout << "Block Index " << blockIndex << std::endl;
	
	const char * lightPropertyComponents[] = { "lightSourcePosition", "lightIntensity","ambientIntensity"};
	glWrapper::UBO lightPropertyBlock{ PROGRAM_HANDLER, "lightProperty", lightPropertyComponents, 3, 1};
	glWrapper::updateUBOComponent(lightPropertyBlock, "lightSourcePosition", value_ptr(pointLight.position), sizeof(float)*4);
	glWrapper::updateUBOComponent(lightPropertyBlock, "lightIntensity", value_ptr(pointLight.intensity), sizeof(float)*4);
	glWrapper::updateUBOComponent(lightPropertyBlock, "ambientIntensity", value_ptr(ambientLight.intensity), sizeof(float)*4);
	lightPropertyBlock.sendFirstTimeDataToUBO(GL_DYNAMIC_DRAW);
	uniformBlocks.insert(std::pair<const std::string, glWrapper::UBO>("lightProperty", lightPropertyBlock));
	
	const char * matrixBlockComponents[] = { "viewMat", "projection" };
	glWrapper::UBO matrixBlock{ PROGRAM_HANDLER, "MatrixBlock",  matrixBlockComponents, 2, 0};
	glWrapper::updateUBOComponent(matrixBlock, "viewMat", value_ptr(CONSTANT::IDENTITY_MATRIX), sizeof(float) * 16);
	glWrapper::updateUBOComponent(matrixBlock, "projection", value_ptr(CONSTANT::PROJECTION_MATRIX), sizeof(float) * 16);
	matrixBlock.sendFirstTimeDataToUBO(GL_DYNAMIC_DRAW);
	uniformBlocks.insert(std::pair<const std::string, glWrapper::UBO>("MatrixBlock", matrixBlock));
}

void Scene::update(long long elapsedMilliseconds) {
	Frustum viewFrustum{ *camera };
	for (auto & modelEntities: entityRenderer.entities){
		modelEntities.second.clear(); // clearing all objects being rendered in 
		//last frame to make way for all objects to be rendered in this frame
	}
	octree->update(elapsedMilliseconds/1000.0f, entityRenderer, viewFrustum);
	//entityRenderer.update(elapsedMilliseconds);
}

void Scene::render(long long elapsedMilliseconds) {
	glm::vec3 eyePos = glm::vec3(camera->eyePosition);
	glm::mat4 cameraMat = camera->getViewMatrix();
	glm::mat4 cameraSkyboxMat = glm::mat4(glm::mat3(cameraMat)); 
	glm::vec4 fogColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	// camera mat brings camera to origin then rotate to align look, right, up with x, y, -z. 

	terrainRenderer.useTerrainShader();	

	/*glWrapper::UBO & lightPropertyBlock = uniformBlocks["lightProperty"];
	glBindBuffer(GL_UNIFORM_BUFFER, lightPropertyBlock.uboIndex);
	glWrapper::updateUBOComponent(lightPropertyBlock, "lightSourcePosition", value_ptr(pointLight.position), sizeof(float)*4);
	glWrapper::updateUBOComponent(lightPropertyBlock, "lightIntensity", value_ptr(pointLight.intensity), sizeof(float)*4);
	glWrapper::updateUBOComponent(lightPropertyBlock, "ambientIntensity", value_ptr(ambientLight.intensity), sizeof(float)*4);
	lightPropertyBlock.sendUpdatedDataToUBO();
	glWrapper::UBO & matrixBlock = uniformBlocks["MatrixBlock"];
	glBindBuffer(GL_UNIFORM_BUFFER, matrixBlock.uboIndex);
	glWrapper::updateUBOComponent(matrixBlock, "viewMat", value_ptr(cameraMat), sizeof(float) * 16);
	glWrapper::updateUBOComponent(matrixBlock, "projection", value_ptr(PROJECTION_MATRIX), sizeof(float) * 16);
	matrixBlock.sendUpdatedDataToUBO();*/
	
	glUniformMatrix4fv(glGetUniformLocation(terrainRenderer.terrainShader.PROGRAM,"projection"), 1, GL_FALSE, value_ptr(CONSTANT::PROJECTION_MATRIX));
	glUniform4fv(glGetUniformLocation(terrainRenderer.terrainShader.PROGRAM, "lightSourcePosition"), 1, value_ptr(pointLight.position));
	glUniform4fv(glGetUniformLocation(terrainRenderer.terrainShader.PROGRAM, "lightIntensity"), 1, value_ptr(pointLight.intensity));
	glUniform4fv(glGetUniformLocation(terrainRenderer.terrainShader.PROGRAM, "ambientIntensity"), 1, value_ptr(ambientLight.intensity));
	glUniformMatrix4fv(glGetUniformLocation(terrainRenderer.terrainShader.PROGRAM, "viewMat"), 1, GL_FALSE, value_ptr(cameraMat));
	loadFogToShader(terrainRenderer.terrainShader.PROGRAM, fogColor, 1.5f, 0.0035f);
	terrainRenderer.render();
	
	entityRenderer.useEntityShader();
	
	glUniformMatrix4fv(glGetUniformLocation(entityRenderer.entityShader.PROGRAM, "projection"), 1, GL_FALSE, value_ptr(CONSTANT::PROJECTION_MATRIX));
	glUniform4fv(glGetUniformLocation(entityRenderer.entityShader.PROGRAM, "lightSourcePosition"), 1, value_ptr(pointLight.position));
	glUniform4fv(glGetUniformLocation(entityRenderer.entityShader.PROGRAM, "lightIntensity"), 1, value_ptr(pointLight.intensity));
	glUniform4fv(glGetUniformLocation(entityRenderer.entityShader.PROGRAM, "ambientIntensity"), 1, value_ptr(ambientLight.intensity));
	glUniformMatrix4fv(glGetUniformLocation(entityRenderer.entityShader.PROGRAM, "viewMat"), 1, GL_FALSE, value_ptr(cameraMat));
	loadFogToShader(entityRenderer.entityShader.PROGRAM, fogColor, 1.5f, 0.0035f);
	entityRenderer.render(elapsedMilliseconds, chosenOnes);

	healthbarRenderer.useHealthBarShader();
	glUniformMatrix4fv(glGetUniformLocation(healthbarRenderer.healthbarShader.PROGRAM, "projection"), 1, GL_FALSE, value_ptr(CONSTANT::PROJECTION_MATRIX));
	glUniformMatrix4fv(glGetUniformLocation(healthbarRenderer.healthbarShader.PROGRAM, "viewMat"), 1, GL_FALSE, value_ptr(cameraMat));
	healthbarRenderer.render(chosenOnes);

	boundingboxRenderer.useBoundingBoxShader();
	glUniformMatrix4fv(glGetUniformLocation(boundingboxRenderer.boundingboxShader.PROGRAM, "projection"), 1, GL_FALSE, value_ptr(CONSTANT::PROJECTION_MATRIX));
	glUniformMatrix4fv(glGetUniformLocation(boundingboxRenderer.boundingboxShader.PROGRAM, "viewMat"), 1, GL_FALSE, value_ptr(cameraMat));
	boundingboxRenderer.render();

	guiRenderer.useGUIShader();
	guiRenderer.render(elapsedMilliseconds);
}

void Scene::loadFogToShader(unsigned int PROGRAM, glm::vec4 fogColor, float gradient, float density) {
	glUniform1f(glGetUniformLocation(PROGRAM, "density"), density);
	glUniform1f(glGetUniformLocation(PROGRAM, "gradient"), gradient);
	glUniform4fv(glGetUniformLocation(PROGRAM, "fogColor"), 1, value_ptr(fogColor));
}

IntersectionRecord Scene::mousePick(Ray & ray) {
	IntersectionRecord intersectionRecord{};
	octree->hitRecursive(ray, &intersectionRecord);
	if (intersectionRecord.e_ptr == nullptr) {
		ray.intersectPlane(Plane{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, intersectionRecord);
	}
	return intersectionRecord;
}

IntersectionRecord Scene::mouseIntersectTerrain(Ray & r) {
	IntersectionRecord intersectionRecord{};
	r.intersectPlane(Plane{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, intersectionRecord);
	return intersectionRecord;
}

void Scene::handleEvents(std::vector<Action> & actions) {

	for (int i = 0; i < actions.size(); i++) {
		if (actions[i].fromMouse) {
			if (actions[i].key == InputManager::DRAG) {
				if (actions[i].intRanges[4]) { // if finished
					float startX = actions[i].floatRanges[0];
					float startY = actions[i].floatRanges[1];
					int endX = actions[i].intRanges[2];
					int endY = actions[i].intRanges[3];
					std::vector<Entity *> chosens = dragSelect(startX, startY, endX, endY);
					if (chosens.size() > 0) {
						chosenOnes.clear();
						std::cout << "Scene: Drag and clear" << std::endl;
						for (int i = 0; i < chosens.size(); i++) {
							chosenOnes.insert(chosens[i]);
						}
					}
				}
				
			}
			else if (actions[i].key == InputManager::LEFT_PRESS) {
				Ray r = Ray::getMouseRay(actions[i].intRanges[0], actions[i].intRanges[1], camera);
				IntersectionRecord hitRecord = mousePick(r);
				if (hitRecord.e_ptr != nullptr) {
					chosenOnes.clear();
					chosenOnes.insert(hitRecord.e_ptr);
				}

			}
		}
	}

}


std::vector<Entity *> Scene::dragSelect(float startX, float startY, int endX, int endY) {
	std::vector<Entity *> chosens;
	Ray r4 = Ray::getMouseRay(endX, endY, camera);
	IntersectionRecord record4 = mouseIntersectTerrain(r4);
	float ghpX = record4.groundHitPoint.x;
	float ghpY = record4.groundHitPoint.y;
	float minX, maxX, minZ, maxZ;
	if (startX < ghpX) {
		minX = startX;
		maxX = ghpX;
	}
	else {
		minX = ghpX;
		maxX = startX;
	}
	if (startY < ghpY) {
		minZ = startY;
		maxZ = ghpY;
	}
	else {
		minZ = ghpY;
		maxZ = startY;
	}
	octree->dragSelectRecursive(chosens, minX, minZ, maxX, maxZ);
	return chosens;
}