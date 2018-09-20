#pragma once
#include "Util.h"
#include <vector>
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\transform.hpp"
#include "Animator.h"
#include "Model.h"
#include "InputManager.h"
#include "CallbackInterface.h"


// This is the interface for every single thing inside the world, from the player to the tree
// this is an abstract class, not to be instantiated
class Entity:public CallbackInterface {

	public:
		Model * model;
		Animator animator; // even building may need animation later on

		glm::vec3 orientation; // euler angles in radians, though not really. 
		//for example, orientation.y isn't the angle it makes with y coordinate basis
		// but it actually is the angle it rotates around y axis
		glm::vec3 position;
		glm::vec3 scale;
		BoundingBox boundingBox;

		Entity(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale, int id);
		
		
		void draw(unsigned int PROGRAM_HANDLER, float elapsedSeconds);
		void loadUniformBoneToShader(unsigned int PROGRAM_HANDLER, std::vector<glm::mat4> & boneTransforms);
		glm::mat4 getModelMat(); // this method is only used for drawing, when model vertices are never changed
		glm::mat4 getNonRotateModelMat(); // this method is used for drawing health bars above units
		
		// return true if object has moved, false otherwise
		virtual bool update(float elapsedSeconds) = 0;
		virtual void moveToward(float x, float z) = 0;
		virtual void handleEvents(std::vector<Action> & actions) = 0;
};