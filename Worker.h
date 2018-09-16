#include "MovingUnit.h"
#include "ID.h"



class Worker :public MovingUnit {

	public:
		static float linearSpeed;
		static float angularSpeed;
		static int maxHealth;
		static int ID;

		Worker(Model * model, glm::vec3 orientation, glm::vec3 position, glm::vec3 scale);
	
		bool update(float elapsedSeconds) override;
		void handleEvents(InputWrapper & inputWrapper) override;
		

};
