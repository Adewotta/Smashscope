#include <grrlib.h>
#include <string.h>
#include <vector>
#include <stdint.h>
#include "./../helpers/controller.h"
#include "MenuEnums.h"
#include "MenuOption.h"
#include "Menu.h"
#include "MenuRunner.h"




MenuRunner::MenuRunner(Menu* menuRoot){
	this->menuRoot = menuRoot;
}

int MenuRunner::runMenu(){
	//menuRoot->update() returns a code for how to handle the update
	uint8_t recursionDepth = 0;
	int updateCode = menuRoot->update(&recursionDepth,1);
	if(updateCode == ENABLE_UPDATE){
		if( passedValueAll(STICKY,60) == RISING){
			this->menuRoot->decrementSelection();
		}	
		if( passedValueAll(STICKY,-60) == FALLING){
			this->menuRoot->incrementSelection();
		}
		if(PAD_ButtonsDownAll() & PAD_BUTTON_A){
			MenuOption* selection = this->menuRoot->getSelection();
				if(selection->getType()==SUB_MENU_OPTION){
					this->menuRoot = (Menu*)selection;

				}
			}
		}
		if(PAD_ButtonsDownAll() & PAD_BUTTON_B){
			MenuOption* parent = this->menuRoot->getParent();
			if(parent != NULL){
				this->menuRoot = (Menu*)parent;
			}
			else{
				return 1;
			}
		}
	
	return 0;
}


void MenuRunner::drawMenu(int x, int y){
	this->menuRoot->drawMenu(x,y);
}

