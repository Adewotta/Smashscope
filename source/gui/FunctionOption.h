#pragma once
#include <string>
#include "MenuOption.h"
#include <functional>
class FunctionOption : public MenuOption{
	public:
		FunctionOption(std::string label, std::string description, 
				std::function<void(void)> callback);
	
		//Functions
		int update(void* argc, int argv);
		//Getters
		//std::string getLabel();
		//std::string getDescription();
		//int getType();
		//Setters
private:
	std::function<void(void)> callback;
};


