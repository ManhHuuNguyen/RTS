#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <map>
#include "Texture.h"
#include "Shader.h"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"

class GUI {
	
	public:
		Texture * texture;
		glm::vec2 position; // considered as center of rectangle
		glm::vec2 scale;

		GUI(Texture * texture, int startX, int startY, int endX, int endY); // pixel on screen
		glm::mat4 getModelMat();
		void updateModelMat(int startX, int startY, int endX, int endY); // pixel on screen
};

class DragSquare: public GUI {

};

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
		void render(long long elapsedMilliSeconds);

};