/*===========================================
    SmashScope
    Wii Homebrew for testing and
    debugging gamecube controllers
============================================*/
#include <grrlib.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <functional>
#include <fat.h>

#include "globals/fonts.h"
#include "scientifica_ttf.h"
#include "helpers/controller.h"
#include "tools/pollTest.h"
#include "config/config.h"
#include "tools/serialRead.h"
#include "tools/debug.h"

#include "gui/MenuOption.h"
#include "gui/Menu.h"
#include "gui/MenuRunner.h"
#include "gui/FunctionOption.h"


void def(){
	return;
}

int main() {
	GRRLIB_Init();
	PAD_Init();

	/*
	*Iniitalize global fonts
	**/
	std::function<void(void)> test;
	menuFont = GRRLIB_LoadTTF(scientifica_ttf,scientifica_ttf_size);
	
	Menu menu1 = Menu("Root", "Root DESC");
	Menu devMenu = Menu("Controller Development Tools", "A collection of tools for testing new third party controllers");
	
	FunctionOption Func1 = FunctionOption("UNIMPLEMENTED","TEST",nullptr);
	FunctionOption Func2 = FunctionOption("UNIMPLEMENTED","TEST",nullptr);	
	FunctionOption Func3 = FunctionOption("UNIMPLEMENTED","",nullptr);
	FunctionOption Func4 = FunctionOption("DEBUG","Dumps information to the screen",&runDebugInfo);	
	
	FunctionOption DevFunc1 = FunctionOption("Polling Speed Test","TEST",&runPollTest);
	FunctionOption DevFunc2 = FunctionOption("Goomwave Serial Dump (WIP)","TEST",&runSerialReader);

	menu1.addItem((MenuOption*)&Func1);
	menu1.addItem((MenuOption*)&Func2);
	menu1.addItem((MenuOption*)&Func3);	
	menu1.addItem((MenuOption*)&devMenu);
	menu1.addItem((MenuOption*)&Func4);	
	

	devMenu.addItem((MenuOption*)&DevFunc1);
	devMenu.addItem((MenuOption*)&DevFunc2);

	MenuRunner mainMenu = MenuRunner(&menu1);
	/*
	 * Initialize display settings
	 **/	
	GRRLIB_Settings.antialias = true;
	GRRLIB_SetBackgroundColour(0x10,0x10,0x20,0xff);
	GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);
	

	/*
	 *Load configuration files
	 */

	loadConfig();
	resetConfig();
	saveConfig();
	while(1) {
		PAD_ScanPads();	
		updateController();
		GRRLIB_2dMode();
		mainMenu.runMenu();	
		mainMenu.drawMenu(60,140);


		//		int exitCode = menu.Update();
//		if( exitCode != 0 ) break;
		GRRLIB_Render();
	}
	
	/*
	 *Release global fonts
	 */
	GRRLIB_FreeTTF(menuFont);
	
	GRRLIB_Exit();
	return 0;
}

