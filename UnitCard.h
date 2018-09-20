#pragma once
#include "GUI.h"
class UnitCard:public GUI {

	public:
		UnitCard(Texture * texture, int startX, int startY, int endX, int endY);
};