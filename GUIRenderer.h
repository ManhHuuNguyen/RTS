#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <map>
#include "Shader.h"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "ActiveGUI.h"
#include "UnitCard.h"
#include <set>
#include "Entity.h"




class GUIRenderer {
	// top left of screen is 0, 0
	// vertex positions -1, 1, -1, -1, 1, 1, 1, -1, triangle strips
	public:
		static float vertices[8];
		std::map<Texture *, std::vector<GUI *>> guiMap;
		Shader guiShader;
		GLuint vao;
		GLuint vbo;

		GUIRenderer();
		void useGUIShader();
		void addGUI(GUI * gui);
		void render(long long elapsedMilliSeconds, std::set<Entity *> & chosenOnes);

};