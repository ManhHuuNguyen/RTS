#pragma once

#include "glm\glm.hpp"
#include<glm/gtc/quaternion.hpp>
#include<glm/common.hpp>

#include <iostream>
#include "Util.h"
#include "glm\gtx\transform.hpp"
#include "CallbackInterface.h"
#include "ID.h"


class Camera :public CallbackInterface {


public:

	glm::vec4 eyePosition;
	glm::vec3 look;
	glm::vec3 right;
	glm::vec3 up;
	static float cameraSpeed;

	Camera(); // needed for classes that inherit this class

	Camera(float eyeX, float eyeY, float eyeZ, float spotX, float spotY, float spotZ, float upX, float upV, float upZ);

	Camera(float radianAngle, float height);

	void rotate(glm::vec3 axis, float theta);

	void normalizeLRU();

	void moveByMouse(int mouseX, int mouseY);

	glm::mat4 getViewMatrix();

	void handleEvents(std::vector<Action> & actions) override;
};