#include "MovingUnit.h"




class Worker :public MovingUnit {

	public:
		static float linearSpeed;
		static float angularSpeed;
		static int maxHealth;

		Worker(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale);
	
		bool update(float elapsedSeconds) override;
		void handleEvents(InputWrapper & inputWrapper) override;
		

};
