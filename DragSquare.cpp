#include "DragSquare.h"


DragSquare::DragSquare(Texture * texture):GUI(texture, 0, 0, 0, 0) {

}

void DragSquare::update(InputWrapper & inputWrapper, Camera * cam) {
	computeStartPointPixel(cam->getViewMatrix());
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
						mouseRay.intersectPlane(Plane{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, record);
						start(record.groundHitPoint.x, record.groundHitPoint.y);
						updateModelMat(input.ranges[0], input.ranges[1], input.ranges[2], input.ranges[3]);
					}
					else {
						updateModelMat(startPointInPixel.x, startPointInPixel.y, input.ranges[2], input.ranges[3]);
					}
				}
			}
		}
	}
}

void DragSquare::start(float startX, float startY) {
	begin = true;
	startPoint = glm::vec2(startX, startY);
}

void DragSquare::computeStartPointPixel(glm::mat4 viewMatrix) {
	glm::vec4 sp = glm::vec4(startPoint.x, 0.0f, startPoint.y, 1.0f); // startPoint.y is actually z
	sp = CONSTANT::PROJECTION_MATRIX*viewMatrix * sp;
	sp = sp / sp.w; // perspective division
	startPointInPixel.x = Util::mapInterval(sp.x, -1.0f, 1.0f, 0.0f, (float)CONSTANT::WIDTH_DISPLAY);
	startPointInPixel.y = Util::mapInterval(sp.y, -1.0f, 1.0f, (float)CONSTANT::HEIGHT_DISPLAY, 0.0f);
	

}