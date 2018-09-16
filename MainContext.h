#pragma once
#include "Context.h"
#include "Scene.h"
#include <map>
#include "DragSquare.h"

class MainContext:public Context{
	public:
		Camera * camera;
		DragSquare * dragSquare;
		Scene * scene;
		std::map<int, std::vector<InputMapObj>> inputMapper; // for small number, vector works faster than map
		MainContext(Camera * camera, DragSquare * dragSquare, Scene * scene);

		MainContext(const char * filename);
		void callBack(InputWrapper & inputWrapper) override; 
};