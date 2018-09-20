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
		std::map<std::string, std::vector<int>> kbInputMapper; // for small number, vector works faster than map
		std::map<std::string, std::vector<int>> mouseInputMapper;

		MainContext(Camera * camera, DragSquare * dragSquare, Scene * scene);

		void readContextFile(const char * filename);
		void callBack(InputWrapper & inputWrapper) override; 
		std::map<int, std::vector<Action>> mapAction(InputWrapper & inputWrapper);
};