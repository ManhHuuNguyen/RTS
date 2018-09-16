#include "GUIRenderer.h"

GUI::GUI(Texture * texture, int startX, int startY, int endX, int endY) {
	this->texture = texture;
	updateModelMat(startX, startY, endX, endY);
}

glm::mat4 GUI::getModelMat() {
	return glm::translate(glm::vec3(position, 0.0f)) * glm::scale(glm::vec3(scale, 1.0f));
}

void GUI::handleEvents(InputWrapper & inputWrapper) {

}

void GUI::updateModelMat(int startX, int startY, int endX, int endY) {// convert screen pixel to normalized device coordinate (-1, 1)
	int width = abs(startX - endX);
	int height = abs(startY - endY);
	int halfWidthDisplay = CONSTANT::WIDTH_DISPLAY / 2;
	int halfHeightDisplay = CONSTANT::HEIGHT_DISPLAY / 2;
	float ax = (float)width / CONSTANT::WIDTH_DISPLAY;
	float ay = (float)height / CONSTANT::HEIGHT_DISPLAY;
	scale = glm::vec2(ax, ay);
	

	float startxNDC = Util::mapInterval((float)startX, 0.0f, (float)CONSTANT::WIDTH_DISPLAY, -1.0f, 1.0f);// where i want to get to
	float startyNDC = Util::mapInterval((float)startY, 0.0f, (float)CONSTANT::HEIGHT_DISPLAY, 1.0f, -1.0f);
	// range of startyNDC goes from 1.0f to -1.0f because direction of y axis of normalized device coordinate and screen coordinate are opposite
	float xNDC, yNDC; // where I am at
	if (startX < endX) {
		if (startY < endY) {
			xNDC = -ax;
			yNDC = ay;
		}
		else {
			xNDC = -ax;
			yNDC = -ay;
		}
	}
	else {
		if (startY < endY) {
			xNDC = ax;
			yNDC = ay;
		}
		else {
			xNDC = ax;
			yNDC = -ay;
		}
	}
	position = glm::vec2(startxNDC - xNDC, startyNDC - yNDC);
}

float GUIRenderer::vertices[8] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };

GUIRenderer::GUIRenderer() :guiShader("GUIVertexShader.vert", "GUIFragmentShader.frag") {
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


void GUIRenderer::render(long long elapsedMilliSeconds) {
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