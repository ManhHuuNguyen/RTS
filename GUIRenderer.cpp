#include "GUIRenderer.h"



float GUIRenderer::vertices[8] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };

GUIRenderer::GUIRenderer() :guiShader("shaders/GUIVertexShader.vert", "shaders/GUIFragmentShader.frag") {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
}


void GUIRenderer::useGUIShader() {
	glUseProgram(guiShader.PROGRAM);
}

void GUIRenderer::addGUI(GUI * gui) {
	Texture * t = gui->texture;
	guiMap[t].push_back(gui);
}


void GUIRenderer::render(long long elapsedMilliSeconds, std::set<Entity *> & chosenOnes) {
	glBindVertexArray(vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto & textureGUI : guiMap) {
		std::vector<GUI *> & allGUIs = textureGUI.second;
		if (allGUIs.size() > 0) {
			for (int i = 0; i < allGUIs.size(); i++) {
				glUniformMatrix4fv(glGetUniformLocation(guiShader.PROGRAM, "transformMat"), 1, GL_FALSE, value_ptr(allGUIs[i]->getModelMat()));
				glUniform1ui(glGetUniformLocation(guiShader.PROGRAM, "hasTexture"), 1u);
				glActiveTexture(GL_TEXTURE0 + allGUIs[i]->texture->bindingPort);
				glBindTexture(GL_TEXTURE_2D, allGUIs[i]->texture->texID);
				glUniform1i(glGetUniformLocation(guiShader.PROGRAM, "guiTexture"), allGUIs[i]->texture->bindingPort);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}
	}
	glDisable(GL_BLEND);

}