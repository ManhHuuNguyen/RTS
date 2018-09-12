#pragma once
#include "GL\glew.h"
#include "Shader.h"
#include <set>
#include "glm\gtc\type_ptr.hpp"
#include "Entity.h"

class HealthBarRenderer {

	public:
		GLuint vao;
		GLuint vbo;
		Shader healthbarShader;

		HealthBarRenderer();
		
		void useHealthBarShader();
		void render(std::set<Entity *> & chosenOnes);
		~HealthBarRenderer();
};