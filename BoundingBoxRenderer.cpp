#include "BoundingBoxRenderer.h"


GLuint BoundingBoxRenderer::indices[24] = {
	0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 7, 7, 3, 7, 6, 6, 5, 5, 4, 5, 1, 6, 2
};

BoundingBoxRenderer::BoundingBoxRenderer():boundingboxShader("shaders/BoundingBoxVertexShader.vert", "shaders/BoundingBoxFragmentShader.frag") {
	addBBToVAO();
}

void BoundingBoxRenderer::useBoundingBoxShader() {
	glUseProgram(boundingboxShader.PROGRAM);
}

void BoundingBoxRenderer::render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(bbVAO);
	
	for (int i = 0; i < boxOfEntities.size(); i++) {
		BoundingBox & box = boxOfEntities[i]->boundingBox;
		glm::mat4 modelMat = boxOfEntities[i]->getModelMat();
		std::vector<glm::vec3> vertices = box.move(modelMat).getCorners();
		glBindBuffer(GL_ARRAY_BUFFER, bbVBO[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), &vertices[0]);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBoxRenderer::addBBToVAO() {
	glGenVertexArrays(1, &bbVAO);
	glGenBuffers(2, bbVBO);
	glBindVertexArray(bbVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bbVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bbVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

BoundingBoxRenderer::~BoundingBoxRenderer() {
	glDeleteVertexArrays(1, &bbVAO);
	glDeleteBuffers(1, bbVBO);
}