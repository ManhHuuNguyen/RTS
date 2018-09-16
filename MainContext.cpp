#include "MainContext.h"

MainContext::MainContext(const char * filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if (!result){
		std::cout << "XML [" << filename << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << (filename + result.offset) << "]\n\n";
		return;
	}
	pugi::xml_node startNode = doc.child("MainContext");
	for (pugi::xml_node object = startNode.child("object"); object; object = object.next_sibling()) {
		int object_id = object.attribute("id").as_int();
		for (pugi::xml_node action = object.child("action"); action; action = action.next_sibling()) {
			pugi::xml_node key_node = action.child("key");
			if (key_node.attribute("type").as_string() == std::string{ "char" }) {
				char key = key_node.text().as_string()[0];
				int value = action.child("value").text().as_int();
				int numFollow = action.child("num-follow").text().as_int();
				InputMapObj a{true, key, -1, value, numFollow};
				inputMapper[object_id].push_back(a);
			}
			else if (key_node.attribute("type").as_string() == std::string{ "int" }) {
				int key = key_node.text().as_int();
				int value = action.child("value").text().as_int();
				int numFollow = action.child("num-follow").text().as_int();
				InputMapObj a{false, ' ', key, value, numFollow};
				inputMapper[object_id].push_back(a);
			}
		}
	}
	for (auto obj : inputMapper) {
		std::vector<InputMapObj> & actions = obj.second;
		std::cout << obj.first << std::endl;
		for (int i = 0; i < actions.size(); i++) {
			if (actions[i].isChar) {
				std::cout << actions[i].charKey << " ";
			}
			else {
				std::cout << actions[i].intKey << " ";
			}
			std::cout << actions[i].value << " " << actions[i].numFollow << std::endl;
		}
	}
}

MainContext::MainContext(Camera * camera, DragSquare * dragSquare, Scene * scene) :Context(true) {
	this->camera = camera;
	this->dragSquare = dragSquare;
	this->scene = scene;
}

void MainContext::callBack(InputWrapper & inputWrapper) {
	camera->handleEvents(inputWrapper);
	dragSquare->handleEvents(inputWrapper);
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		if (inputWrapper.inputs[i].fromMouse) {
			if (inputWrapper.inputs[i].inputID == InputManager::DRAG) {
				if (inputWrapper.inputs[i].ranges[4]) { // if finished
					int startX = inputWrapper.inputs[i].ranges[0];
					int startY = inputWrapper.inputs[i].ranges[1];
					int endX = inputWrapper.inputs[i].ranges[2];
					int endY = inputWrapper.inputs[i].ranges[3];
					Ray r1 = getMouseRay(startX, startY, camera);
					Ray r2 = getMouseRay(startX, endY, camera);
					Ray r3 = getMouseRay(endX, startY, camera);
					Ray r4 = getMouseRay(endX, endY, camera);
					std::vector<Entity*> chosens = scene->dragSelect(r1, r2, r3, r4);
					if (chosens.size() > 0) {
						scene->chosenOnes.clear();
						for (int i = 0; i < chosens.size(); i++) {
							scene->chosenOnes.insert(chosens[i]);
						}
					}
				}
			}
			else if (inputWrapper.inputs[i].inputID == InputManager::LEFT_PRESS) {
				Ray r = getMouseRay(inputWrapper.inputs[i].ranges[0], inputWrapper.inputs[i].ranges[1], camera);
				IntersectionRecord hitRecord = scene->mousePick(r);
				if (hitRecord.e_ptr != nullptr) {
					scene->chosenOnes.clear();
					scene->chosenOnes.insert(hitRecord.e_ptr);
				}
			}
			else if (inputWrapper.inputs[i].inputID == InputManager::RIGHT_PRESS) {
				Ray r = getMouseRay(inputWrapper.inputs[i].ranges[0], inputWrapper.inputs[i].ranges[1], camera);
				IntersectionRecord hitRecord = scene->mousePick(r);
				if (scene->chosenOnes.size() > 0 && hitRecord.e_ptr == nullptr && hitRecord.groundHitPoint.x <= CONSTANT::WORLD_SIZE) {
					Input action{ InputManager::RIGHT_PRESS, 2, true };
					action.floatRanges.push_back(hitRecord.groundHitPoint.x);
					action.floatRanges.push_back(hitRecord.groundHitPoint.y);
					inputWrapper.inputs[i] = action;
				}
			}
		}
	}	
	for (auto pickedEntity : scene->chosenOnes) {
		pickedEntity->handleEvents(inputWrapper);
	}
}