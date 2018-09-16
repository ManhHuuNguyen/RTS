#include "Camera.h"

float Camera::cameraSpeed = 10*0.00005f;
int Camera::ID = ID::CAMERA_ID;

Camera::Camera() { // it will be a camera looking straight downward
	eyePosition = glm::vec4(0.0f, 80.0f, 0.0f, 1.0f);
	/*look = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);*/
	look = glm::vec3(0.0f, -1.0f, 0.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, -1.0f);
	
}

Camera::Camera(float radianAngle, float height) { // my camera at initial position looks straight at 0,0,0
	float z = height / tanf(radianAngle);//tan(angle) = height/z because there is no x, y component at first. i do it for ease of computing bc im lazy
	eyePosition = glm::vec4(0.0f, height, z, 1.0f);
	look = -glm::normalize(glm::vec3(eyePosition));
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::cross(right, look); 
}

Camera::Camera(float eyeX, float eyeY, float eyeZ, float spotX, float spotY, float spotZ, float upX, float upY, float upZ) {
	eyePosition = glm::vec4(eyeX, eyeY, eyeZ, 1.0f);
	look = glm::vec3(spotX, spotY, spotZ) - glm::vec3(eyePosition);
	up = glm::vec3(upX, upY, upZ);
	normalizeLRU();
}

void Camera::rotate(glm::vec3 axis, float theta) {
	glm::mat4 rotationMat = glm::translate(glm::vec3(eyePosition));
	rotationMat = rotationMat * glm::rotate(theta, axis);
	rotationMat = rotationMat * glm::translate(-glm::vec3(eyePosition));
	look = glm::vec3(rotationMat * glm::vec4(look, 0.0f));
	right = glm::vec3(rotationMat * glm::vec4(right, 0.0f));
	up = glm::vec3(rotationMat * glm::vec4(up, 0.0f));
}

void Camera::normalizeLRU() {
	look = normalize(look);
	right = glm::cross(look, up);
	right = normalize(right);
	up = glm::cross(right, look);
}

// im doing this because of the way I'm calculating look, right, up vectors of camera. It should be changed 
void Camera::moveByMouse(int mouseX, int mouseY) {
	float relX = mouseX - CONSTANT::WIDTH_DISPLAY / 2.0f;
	float relY = mouseY - CONSTANT::HEIGHT_DISPLAY / 2.0f;

	if (mouseX < 0.15f * CONSTANT::WIDTH_DISPLAY || mouseX > 0.85f * CONSTANT::WIDTH_DISPLAY) {
		eyePosition.x += relX * cameraSpeed;
	}
	if (mouseY < 0.15f * CONSTANT::HEIGHT_DISPLAY || mouseY > 0.85f * CONSTANT::HEIGHT_DISPLAY) {
		eyePosition.z += relY * cameraSpeed;
	}
}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 eyePos = glm::vec3(eyePosition);
	return  glm::lookAt(eyePos, eyePos + look, up);
}

void Camera::handleEvents(InputWrapper & inputWrapper) {
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		if (!inputWrapper.inputs[i].fromMouse) {
			switch (inputWrapper.inputs[i].inputID) {
			case SDLK_w:
				rotate(glm::vec3(1.0f, 0.0f, 0.0f), -0.1f);
				break;
			case SDLK_s:
				rotate(glm::vec3(1.0f, 0.0f, 0.0f), 0.1f);
				break;
			case SDLK_a:
				rotate(glm::vec3(0.0f, 1.0f, 0.0f), -0.1f);
				break;
			case SDLK_d:
				rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.1f);
				break;
			}
		}
		else {
			if (inputWrapper.inputs[i].inputID == InputManager::CURRENT_POS) {
				moveByMouse(inputWrapper.inputs[i].ranges[0], inputWrapper.inputs[i].ranges[1]);
			}
		}
		
	}
}

