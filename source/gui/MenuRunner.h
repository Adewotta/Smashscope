#pragma once
#include <string>
#include <vector>
#include "Menu.h"

class MenuRunner{
	public:
		
		MenuRunner(Menu* menuRoot);
		int runMenu();
		void drawMenu(int x, int y);
	private:
		Menu* menuRoot;
};
