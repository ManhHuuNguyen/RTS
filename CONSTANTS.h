#pragma once
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

namespace CONSTANT {
	extern glm::vec3 COLOR_RED;
	extern glm::vec3 COLOR_GREEN;
	extern glm::vec3 COLOR_BLUE;
	extern glm::vec3 COLOR_YELLOW;
	extern glm::mat4 IDENTITY_MATRIX;
	extern float MARGIN_OF_ERROR;
	extern int WIDTH_DISPLAY;
	extern int HEIGHT_DISPLAY;
	extern float fov;
	extern float aspectRatio; // WIDTH/HEIGHT
	extern float zNear;
	extern float zFar;
	extern glm::mat4 PROJECTION_MATRIX;
	extern glm::mat4 INV_PROJECTION_MATRIX;
	extern float WORLD_SIZE;
	extern float kEpsilon; // small number, treat as 0
	extern float MT_PI;	//somehow m_pi cause compile error, weird
}
