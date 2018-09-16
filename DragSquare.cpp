#include "DragSquare.h"

int DragSquare::ID = ID::DRAG_SQUARE_ID;


DragSquare::DragSquare(Texture * texture):GUI(texture, 0, 0, 0, 0) {

}

void DragSquare::handleEvents(InputWrapper & inputWrapper) {
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		if (inputWrapper.inputs[i].fromMouse) {
			if (inputWrapper.inputs[i].inputID == InputManager::DRAG) {
				if (inputWrapper.inputs[i].ranges[4]) { // if finished
					updateModelMat(0, 0, 0, 0);
					begin = false;
				}
				else {
					updateModelMat(inputWrapper.inputs[i].ranges[0], inputWrapper.inputs[i].ranges[1], inputWrapper.inputs[i].ranges[2], inputWrapper.inputs[i].ranges[3]);
				}
			}
		}
	}
}