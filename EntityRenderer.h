#pragma once
#include "glm\glm.hpp"
#include<glm/gtc/quaternion.hpp>
#include<glm/common.hpp>
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "Entity.h"
#include <map>
#include <vector>
#include "Shader.h"



class EntityRenderer {
	public:
		std::map<Model *, std::vector<Entity *>> entities;
		Shader entityShader;
		
		EntityRenderer();
		void useEntityShader();
		void addEntity(Entity * e);
		void render(long long elapsedMilliSeconds, std::set<Entity*> & chosenOnes);
};

