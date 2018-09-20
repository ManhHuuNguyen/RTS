#include "Entity.h"

Entity::Entity(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale, int id):animator(model), CallbackInterface(id) {
	ModelData & m = model->modelData;
	boundingBox = BoundingBox{m.minX, m.maxX, m.minY, m.maxY, m.minZ, m.maxZ };
	
	this->model = model;
	this->orientation = orientation;
	this->position = position;
	this->scale = scale;
	glm::mat4 transformBB = getModelMat();	
}

void Entity::loadUniformBoneToShader(unsigned int PROGRAM_HANDLER, std::vector<glm::mat4> & boneTransforms) {
	glUniformMatrix4fv(glGetUniformLocation(PROGRAM_HANDLER, "joints"), boneTransforms.size(), GL_FALSE, value_ptr(boneTransforms[0]));	
}

void Entity::draw(unsigned int PROGRAM_HANDLER, float elapsedSeconds) {
	glm::mat4 modelMat = getModelMat();
	int count = 0;

	std::vector<glm::mat4> boneTransforms = animator.updateAnimation(elapsedSeconds);
	glUniformMatrix4fv(glGetUniformLocation(PROGRAM_HANDLER, "modelMat"), 1, GL_FALSE, value_ptr(modelMat));
	loadUniformBoneToShader(PROGRAM_HANDLER, boneTransforms);
	for (int i = 0; i < model->materials.size(); i++) {
		Material * material = model->materials[i];
		if (material->diffuseTexture != nullptr) {
			glActiveTexture(GL_TEXTURE0 + material->diffuseTexture->bindingPort);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTexture->texID);
			glUniform1i(glGetUniformLocation(PROGRAM_HANDLER, "textureSampler"), material->diffuseTexture->bindingPort);
			glUniform1ui(glGetUniformLocation(PROGRAM_HANDLER, "hasTexture"), 1u);
		}
		else {
			glUniform3fv(glGetUniformLocation(PROGRAM_HANDLER, "KD"), 1, value_ptr(material->KD));
			glUniform3fv(glGetUniformLocation(PROGRAM_HANDLER, "KA"), 1, value_ptr(material->KA));
			glUniform1ui(glGetUniformLocation(PROGRAM_HANDLER, "hasTexture"), 0u);
		}
		int size = material->mesh.vertices.size();
		glDrawArrays(GL_TRIANGLES, count, size);
		count += size;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

glm::mat4 Entity::getModelMat() {
	glm::vec3 centerOfModel = model->modelData.calculateModelCenter();
	glm::vec3 modelFeetTranslate = glm::vec3(0.0f, model->modelData.minY, 0.0f);
	glm::mat4 modelMat = glm::translate(position + centerOfModel - modelFeetTranslate); // translate back to original pos, then translate to position in world, then subtract the height of feet so that model touches the ground
	glm::mat4 rotateX = glm::rotate(orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotateY = glm::rotate(orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotateZ = glm::rotate(orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat *= rotateX;
	modelMat *= rotateY;
	modelMat *= rotateZ;
	modelMat *= glm::scale(scale);
	modelMat *= glm::translate(-centerOfModel); // translate model center to origin to rotate and scale
	return modelMat;
}

glm::mat4 Entity::getNonRotateModelMat() {
	glm::vec3 centerOfModel = model->modelData.calculateModelCenter();
	glm::mat4 modelMat = glm::translate(position + centerOfModel);
	modelMat *= glm::scale(scale);
	modelMat *= glm::translate(-centerOfModel);
	return modelMat;
}

