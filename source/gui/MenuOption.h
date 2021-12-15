#pragma once
#include <string>

class MenuOption{
	public:
		MenuOption(std::string label, std::string description);
		//Functions
		virtual int update(void* argc, int argv);
		//Setters
		void setParent(MenuOption* parent);
		MenuOption* getParent();
		//Getters
		virtual std::string getLabel(bool optionalSelectedFlag = false);
		virtual std::string getDescription();
		int getType();
	protected:
		int type;
		std::string description;
		std::string label;
		MenuOption* parent;
};
