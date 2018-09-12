#pragma once

#include <vector>
#include "Entity.h"
#include "Shader.h"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "Util.h"


class BoundingBoxRenderer {

	public:
		Shader boundingboxShader;
		std::vector<Entity *> boxOfEntities;

		GLuint bbVAO;
		GLuint bbVBO[2];
		static GLuint indices[24];
		
		BoundingBoxRenderer();
		void useBoundingBoxShader();
		void render();
		void addBBToVAO();
		~BoundingBoxRenderer();
};