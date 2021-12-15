#include <string>
#include <functional>
#include "MenuEnums.h"
#include "FunctionOption.h"
#include "grrlib.h"

#include "../helpers/controller.h"


//Constructor
FunctionOption::FunctionOption(std::string label, std::string description,
					std::function<void(void)> callback):	
	MenuOption(label,description){
	this->type = FUNCTION_OPTION;
	this->label = label;
	this->description = description;
	this->callback = callback;
}


//If a is pressed, enter the callback function;
int FunctionOption::update(void* argc, int argv){	
	uint16_t allDown = PAD_ButtonsDownAll();
	if(allDown & PAD_BUTTON_A){	
		if(callback != NULL){
			callback();
		}
	}
	return ENABLE_UPDATE;
}
