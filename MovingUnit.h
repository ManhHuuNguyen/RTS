#pragma once
#include "Entity.h"
#include "Destination.h"


class MovingUnit:public Entity{


	public:

		static float linearSpeed;
		static float angularSpeed;
		static int maxHealth;
		
		Destination desireLocation;
		float linearV = 0.0f;
		float angularV = 0.0f;
		int currentHealth = maxHealth;

		MovingUnit(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale);
		
		void updatePosition(float linearV, float angularV, float elapsedSeconds);
		bool updateVelocity(float * linearV, float * angularV, float linearSpeed, float angularSpeed, float elapsedSeconds);
		void moveToward(float x, float z) override; // objects cannot fly 
		
		virtual bool update(float elapsedSeconds) = 0;
		virtual void handleEvents(InputWrapper & inputWrapper) = 0;
	
		

};