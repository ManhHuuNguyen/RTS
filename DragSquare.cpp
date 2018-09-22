#include "DragSquare.h"


DragSquare::DragSquare(Texture * texture):GUI(texture, 0, 0, 0, 0) {

}

void DragSquare::update(InputWrapper & inputWrapper, Camera * cam) {
	glm::mat4 viewMatrix = cam->getViewMatrix();
	glm::ivec2 spPixel = computeStartPointPixel(viewMatrix);
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		Input & input = inputWrapper.inputs[i];
		if (input.fromMouse) {
			if (input.inputID == InputManager::DRAG) {
				if (input.ranges[4]) { // if finished
					updateModelMat(0, 0, 0, 0);
					begin = false;
				}
				else {
					if (!begin) {
						IntersectionRecord record;
						Ray mouseRay = Ray::getMouseRay(input.ranges[0], input.ranges[1], cam);
						mouseRay.intersectPlane(CONSTANT::TERRAIN_PLAIN, record);
						start(input.ranges[0], input.ranges[1], cam, record.groundHitPoint.x, record.groundHitPoint.y);
						updateModelMat(input.ranges[0], input.ranges[1], input.ranges[2], input.ranges[3]);
					}
					else {
						updateModelMat(spPixel.x, spPixel.y, input.ranges[2], input.ranges[3]);
					}
				}
			}
		}
	}
}

void DragSquare::start(int startX, int startY, Camera * cam, float spWorldX, float spWorldY) {
	begin = true;
	oldStartPoint = glm::ivec2(startX, startY);
	startPointInWorld = glm::vec2(spWorldX, spWorldY);
	cameratMatAtStart = cam->getViewMatrix();
	cameraPosAtStart = cam->eyePosition;
}

glm::ivec2 DragSquare::computeStartPointPixel(glm::mat4 & viewMatrix) {
	glm::vec4 sp = glm::vec4(startPointInWorld.x, 0.0f, startPointInWorld.y, 1.0f); // startPoint.y is actually z
	sp = CONSTANT::PROJECTION_MATRIX*viewMatrix * sp; // should i use perspective or ortho here? if i use ortho, i should change other code for other vertices of the drag square as well
	sp = sp / sp.w; // perspective division is not used if i used ortho
	int x = (int) Util::mapInterval(sp.x, -1.0f, 1.0f, 0.0f, (float)CONSTANT::WIDTH_DISPLAY);
	int y = (int) Util::mapInterval(sp.y, -1.0f, 1.0f, (float)CONSTANT::HEIGHT_DISPLAY, 0.0f);
	return glm::ivec2(x, y);

}