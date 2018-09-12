#pragma once
#include "Camera.h"
#include "Entity.h"

class FollowCamera:public Camera {

	public:
		float distance = 8.0f;
		Entity * followedEntity;

		FollowCamera(Entity * entity);
		void zoom(int direction);
		void update();


};