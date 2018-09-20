#include "DragSquare.h"


DragSquare::DragSquare(Texture * texture):ActiveGUI(texture, 0, 0, 0, 0, ID::DRAG_SQUARE_ID) {

}

void DragSquare::handleEvents(std::vector<Action> & actions) {
	for (int i = 0; i < actions.size(); i++) {
		Action & action = actions[i];
		if (action.fromMouse) {
			if (action.key == InputManager::DRAG) {
				if (action.intRanges[4]) { // if finished
					updateModelMat(0, 0, 0, 0);
					begin = false;
				}
				else {
					updateModelMat(action.intRanges[0], action.intRanges[1], action.intRanges[2], action.intRanges[3]);
				}
			}
		}
	}
}