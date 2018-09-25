#include "MainContext.h"

void MainContext::readContextFile(const char * filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if (!result){
		std::cout << "XML [" << filename << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << (filename + result.offset) << "]\n\n";
		return;
	}
	pugi::xml_node startNode = doc.child("MainContext");
	
	for (pugi::xml_node action = startNode.child("action"); action; action = action.next_sibling()) {
		const char * key = action.attribute("key").as_string();
			
		for (pugi::xml_node object = action.child("object"); object; object = object.next_sibling()) {
			int object_id = object.attribute("id").as_int();
			if (std::string{ action.attribute("type").as_string() } == "kb") {
				kbInputMapper[std::string{ key }].push_back(object_id);
			}
			else {
				mouseInputMapper[std::string{ key }].push_back(object_id);
			}
			
		}
	}
}

MainContext::MainContext(Camera * camera, DragSquare * dragSquare, Scene * scene) :Context(true) {
	this->camera = camera;
	this->dragSquare = dragSquare;
	this->scene = scene;
	readContextFile("contexts/MainContext.xml");
}

void MainContext::callBack(InputWrapper & inputWrapper) {
	camera->update(inputWrapper); 
	dragSquare->update(inputWrapper, camera);

	std::map<int, std::vector<Action>> map = mapAction(inputWrapper);
	if (map.count(scene->id) != 0) {
		scene->handleEvents(map[scene->id]);
	}
	for (auto pickedEntity : scene->chosenOnes) {
		if (map.count(pickedEntity->id) != 0) {
			pickedEntity->handleEvents(map[pickedEntity->id]);
		}
	}
}

std::map<int, std::vector<Action>> MainContext::mapAction(InputWrapper & inputWrapper) {
	std::map<int, std::vector<Action>> actionMap;
	for (int i = 0; i < inputWrapper.inputs.size(); i++) {
		Input & input = inputWrapper.inputs[i];
		
		if (input.fromMouse) {
			if (mouseInputMapper.count(input.inputID) != 0) { // if input is of type main context receives
				Action action{input.inputID, input.fromMouse};
				if (input.inputID == InputManager::DRAG) {
					action.intRanges.push_back(input.ranges[4]); // the value of finished or not
					if (input.ranges[4]) { // if drag is finished
						int startX = dragSquare->oldStartPoint.x;
						int startY = dragSquare->oldStartPoint.y;
						int endX = input.ranges[2];
						int endY = input.ranges[3];
						glm::mat4 inverseViewMat = glm::inverse(dragSquare->cameratMatAtStart);
						Ray r1 = Ray::getMouseRay(startX, startY, inverseViewMat, dragSquare->cameraPosAtStart);
						Ray r2 = Ray::getMouseRay(startX, endY, camera);
						Ray r3 = Ray::getMouseRay(endX, startY, camera);
						Ray r4 = Ray::getMouseRay(endX, endY, camera);

						float minX, minZ, maxX, maxZ;
						IntersectionRecord rec1;
						r1.intersectPlane(CONSTANT::TERRAIN_PLAIN, rec1);
						IntersectionRecord rec2;
						r2.intersectPlane(CONSTANT::TERRAIN_PLAIN, rec2);
						IntersectionRecord rec3;
						r3.intersectPlane(CONSTANT::TERRAIN_PLAIN, rec3);
						IntersectionRecord rec4;
						r4.intersectPlane(CONSTANT::TERRAIN_PLAIN, rec4);
						
						
						minX = std::min(std::min(std::min(rec1.groundHitPoint.x, rec2.groundHitPoint.x), rec3.groundHitPoint.x), rec4.groundHitPoint.x);
						minZ = std::min(std::min(std::min(rec1.groundHitPoint.y, rec2.groundHitPoint.y), rec3.groundHitPoint.y), rec4.groundHitPoint.y);
						maxX = std::max(std::max(std::max(rec1.groundHitPoint.x, rec2.groundHitPoint.x), rec3.groundHitPoint.x), rec4.groundHitPoint.x);
						maxZ = std::max(std::max(std::max(rec1.groundHitPoint.y, rec2.groundHitPoint.y), rec3.groundHitPoint.y), rec4.groundHitPoint.y);
						action.floatRanges.push_back(minX);
						action.floatRanges.push_back(minZ);
						action.floatRanges.push_back(maxX);
						action.floatRanges.push_back(maxZ);
					}
				}
				else if (input.inputID == InputManager::LEFT_PRESS) {
					action.intRanges = input.ranges;

					Ray r = Ray::getMouseRay(input.ranges[0], input.ranges[1], camera);
					IntersectionRecord record;
					r.intersectPlane(CONSTANT::TERRAIN_PLAIN, record);
					action.floatRanges.push_back(record.groundHitPoint.x);
					action.floatRanges.push_back(record.groundHitPoint.y);
				}
				else if (input.inputID == InputManager::RIGHT_PRESS) {
					action.intRanges = input.ranges;

					Ray r = Ray::getMouseRay(input.ranges[0], input.ranges[1], camera);
					IntersectionRecord record;
					r.intersectPlane(CONSTANT::TERRAIN_PLAIN, record);
					action.floatRanges.push_back(record.groundHitPoint.x);
					action.floatRanges.push_back(record.groundHitPoint.y);
				}
				else if (input.inputID == InputManager::CURRENT_POS) {
					action.intRanges = input.ranges;
				}
				std::vector<int> & entity_ids = mouseInputMapper[input.inputID];
				for (int entity_id : entity_ids) {
					actionMap[entity_id].push_back(action);
				}
				
			}
		}
		else { // if it is from keyboard
			if (kbInputMapper.count(input.inputID) != 0) {// if input is of type main context receives
				Action action{ input.inputID, input.fromMouse };
				std::vector<int> & entity_ids = kbInputMapper[input.inputID];
				for (int entity_id : entity_ids) {
					actionMap[entity_id].push_back(action);
				}
			}
		}
	}
	return actionMap;

}