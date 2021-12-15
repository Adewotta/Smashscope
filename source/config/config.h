#pragma once
#include <unordered_map>
#include "../../include/json.hpp"
//Global configuration
using json = nlohmann::json;

extern json config;
void resetConfig();
std::string getConfigString();
std::string loadDefaultString();
std::string saveConfig();
std::string getCode();
std::string  loadConfig();
	

