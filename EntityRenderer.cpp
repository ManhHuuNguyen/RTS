#include "EntityRenderer.h"

EntityRenderer::EntityRenderer() :entityShader("EntityVertexShader2.vert", "EntityFragmentShader2.frag"){
	
}

void EntityRenderer::useEntityShader() {
	glUseProgram(entityShader.PROGRAM);
}



void EntityRenderer::render(long long elapsedMilliSeconds, std::set<Entity*> & chosenOnes) {
	int entityToRender = 0;
	for (auto & modelEntity: entities) {
		std::vector<Entity *> & allEntities = modelEntity.second;
		entityToRender += allEntities.size();
		if (allEntities.size() > 0) {
			glUniform1ui(glGetUniformLocation(entityShader.PROGRAM, "animated"), modelEntity.first->animated);
			glBindVertexArray(modelEntity.first->vao);			
			for (int i = 0; i < allEntities.size(); i++) {
				allEntities[i]->draw(entityShader.PROGRAM, elapsedMilliSeconds / 1000.0f);
			}
		}
	}
	//std::cout << "EntityRenderer: entities num on screen after frustum cull: " << entityToRender << std::endl;
}

void EntityRenderer::addEntity(Entity * e) {
	Model * model = e->model;
	entities[model].push_back(e);
}