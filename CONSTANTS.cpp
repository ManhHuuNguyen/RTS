#include "CONSTANTS.h"

glm::vec3 CONSTANT::COLOR_RED(1.0f, 0.0f, 0.0f);
glm::vec3 CONSTANT::COLOR_GREEN(0.0f, 1.0f, 0.0f);
glm::vec3 CONSTANT::COLOR_BLUE(0.0f, 0.0f, 1.0f);
glm::vec3 CONSTANT::COLOR_YELLOW(1.0f, 1.0f, 0.0f);
glm::mat4 CONSTANT::IDENTITY_MATRIX(1.0f);
float CONSTANT::MARGIN_OF_ERROR = 0.00001f;
int CONSTANT::WIDTH_DISPLAY = 800; // 1366
int CONSTANT::HEIGHT_DISPLAY = 600; // 768
float CONSTANT::fov = 0.85f;
float CONSTANT::aspectRatio = 1.0f;
float CONSTANT::zNear = 0.1f;
float CONSTANT::zFar = 1000.0f;
float CONSTANT::WORLD_SIZE = 3200.0f;
glm::mat4 CONSTANT::PROJECTION_MATRIX = glm::perspective(CONSTANT::fov, CONSTANT::aspectRatio, CONSTANT::zNear, CONSTANT::zFar);
glm::mat4 CONSTANT::INV_PROJECTION_MATRIX = glm::inverse(CONSTANT::PROJECTION_MATRIX);
extern glm::mat4 CONSTANT::ORTHO_MATRIX = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f,0.1f, 100.0f);
float CONSTANT::kEpsilon = 0.000001f;

float CONSTANT::MT_PI = 3.14159265359f;	//!< pi
