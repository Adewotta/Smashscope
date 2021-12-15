#pragma once
#include <string>
#include <vector>
#include "MenuOption.h"


class Menu: public MenuOption{
	public:
		//Constructor
		Menu(std::string label, std::string description);
		//Functions	
		void drawMenu(int x, int y);
		void addItem(MenuOption* option);
		int update(void* argc, int argv);	
		std::vector<MenuOption*> getOptions();
		void incrementSelection();
		void decrementSelection();
		MenuOption* getSelection();
	private:
		int selected;
		std::vector<MenuOption*> children;

};
/*
void Menu::drawMenu(int x, int y){
	int yCoord = y;
	for(int i = 0; i < children.size(); i++){
		item = children[i]->getLabel;
		GRRLIB_PrintfTTF (x,yCoord, menuFont, item->getLabel().c_str(),14, i==selection ? 0x4466FFFF : 0xFFFFFFFF);
		yCoord += 20;
	}
}

*/
