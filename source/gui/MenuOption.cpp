#include <string>
#include "MenuEnums.h"
#include "MenuOption.h"
#include "grrlib.h"
#include "../helpers/controller.h"
#include "MenuOption.h"
#include "MenuEnums.h"

//Constructor 
MenuOption::MenuOption(std::string label, std::string description){
	this->type = BASE_OPTION;
	this->description = description;
	this->label = label;
}
//Return code to enable updates
int MenuOption::update(void* argc, int argv){
	return ENABLE_UPDATE;
}

//


MenuOption* MenuOption::getParent(){
	return this->parent;

}
std::string MenuOption::getLabel(bool optionalSelectedFlag){
	return label;
}
//
std::string MenuOption::getDescription(){
	return description;
}
//
int MenuOption::getType(){
	return type;
}

void MenuOption::setParent(MenuOption* parent){
	this->parent = parent;
}
