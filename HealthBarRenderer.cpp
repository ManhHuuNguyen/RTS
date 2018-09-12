#include "HealthBarRenderer.h"


HealthBarRenderer::HealthBarRenderer() :healthbarShader("HealthBarVertexShader.vert", "HealthBarFragmentShader.frag") {
	float vertices[8] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
}

void HealthBarRenderer::useHealthBarShader() {
	glUseProgram(healthbarShader.PROGRAM);
}

/*
Essentially, I have a square with size 2. So first I have scale it so that its vertices are in same coordinate system with the model,
which I do through using the sizeData member variable. I scale and translate the square so that you can treat it as if it is the model vertices.
I scale so that its length will match the model's width, its height will be .1 of the model's height and it is always 1.2 height above the model
*/
void HealthBarRenderer::render(std::set<Entity *> & chosenOnes) {
	glBindVertexArray(vao);
	for (auto entity: chosenOnes) {
		
		ModelData & modelData = entity->model->modelData;
		glm::vec2 baseCenter = modelData.calculateBaseCenter();
		float baseRadius = modelData.calculateRadius();
		float height = modelData.calculateHeight();
		glm::mat4 transformMat = glm::translate(glm::vec3(baseCenter.x, height * 1.2f, baseCenter.y)) * glm::scale(glm::vec3(baseRadius, 0.1f * height / 2.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(healthbarShader.PROGRAM, "transformMat"), 1, GL_FALSE, value_ptr(transformMat));
		glUniformMatrix4fv(glGetUniformLocation(healthbarShader.PROGRAM, "modelMat"), 1, GL_FALSE, value_ptr(entity->getNonRotateModelMat()));

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

HealthBarRenderer::~HealthBarRenderer() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}