#include <string>
#include <vector>
#include "../globals/fonts.h"
#include "MenuEnums.h"
#include "MenuOption.h"
#include "Menu.h"


MenuOption* Menu::getSelection(){
	return this->children[selected];
}

Menu::Menu(std::string label, std::string description) : MenuOption(label,description){

	this->children = std::vector<MenuOption*>();
	this->parent = NULL;
	this->selected = 0;
	this->type=SUB_MENU_OPTION;
}


void Menu::addItem(MenuOption* option){
	option->setParent(this);
	this->children.push_back(option);
}



void Menu::incrementSelection(){
	if(selected < children.size()-1){
		selected++;
	}

}
void Menu::decrementSelection(){
	if(selected > 0){
		selected--;
	}

}

void Menu::drawMenu(int x, int y){
	for(int i = 0; i < children.size(); i++){
		auto item = children[i];	
		GRRLIB_PrintfTTF (x,y, menuFont, item->getLabel(i==selected).c_str(),14, i==selected ? 0x4466FFFF : 0xFFFFFFFF);
		y += 20;
	}
}



/*
class Menu: MenuOption{
	public:
		//Constructor
		Menu(std::string label, std::string description);
		//Functions
		void addItem(MenuOption* option);
		std::vector<MenuOption*> getOptions();
		int update();
	private:
		int selected;
		std::vector<MenuOption*> children;

}
*/
int Menu::update(void* argc, int argv){
	uint8_t recursionDepth = 0;
	if(argv > 0){
		recursionDepth= *(uint8_t*)(argc);
	}
	if(recursionDepth!=0){
		return ENABLE_UPDATE; 
	}
	else{
		recursionDepth+=1;
		return children[selected]->update(&recursionDepth,1);
	}
	return ENABLE_UPDATE;
}

