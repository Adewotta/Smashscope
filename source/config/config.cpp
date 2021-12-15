
#include "../../include/json.hpp"
#include "default_json.h"
#include "fat.h"
#include <stdio.h>
#include <string>
#include <functional>
using json = nlohmann::json;
std::string errorCode = "Config not Loaded";
json config;
char buffer[4096*8];//Assign 16kb buffer for loading json data


/*Reset configuration to default values 
 * 
 * */
void resetConfig(){

	config = json::parse(default_json);
	return;
}
/*Get the current configuration as a string
 *
 * */
std::string getConfigString(){
	return config.dump();
}
/*Save configuration to :/SmashscopeConfig.json
 *
 */
std::string saveConfig(){
	std::string errorCode = "Config file saved";
	FILE *fp;	
	fp = fopen("SmashscopeConfig.json", "w");
	if(fp != nullptr){
		fprintf(fp,"%s",config.dump().c_str());
		fclose(fp);
		errorCode = "Config file not found, creating config file at device:/SmashscopeConfig.json";
	}else{
		errorCode = "Config file not found, Unable to create file";
	}
	return errorCode;
}
/*Return last read error code as a string
 *
 */
std::string getCode(){
	return errorCode;
}
/*Load configuration data from SmashscopeConfig.json
 *
 */
std::string  loadConfig(){

		if(fatInitDefault()){
			
			FILE *fp;
			fp = fopen ("SmashscopeConfig.json", "r+");
			
			if(fp != nullptr){
				fgets(buffer,4096*8,fp);
				fclose(fp);
				errorCode = "Successfully loaded config data";
				try{
					config = json::parse(buffer);
				}
				catch(const std::exception &exc){
					errorCode = exc.what();;
				}
				//errorCode = buffer;
			}
			else{
				fp = fopen("SmashscopeConfig.json", "w+");
				if(fp != nullptr){
					fprintf(fp,"%s",default_json);
					fclose(fp);
					errorCode = "Config file not found, creating config file at device:/SmashscopeConfig.json";
				}else{
					errorCode = "Config file not found, Unable to create file";
				}
			}
		}
		else{
			errorCode = "fatInitDefault() failure";
		}
	return errorCode;
}


/*
char buffer[4096];//Assign 4kb buffer
	if(fatInitDefault()){
		
		FILE *fp;
		fp = fopen ("SmashscopeConfig.json", "r+");
		
		if(fp != nullptr){
			fgets(buffer,4096,fp);
			fclose(fp);
		}
		else{
			fp = fopen("SmashscopeConfig.json", "w+");
			if(fp != nullptr){
				fputs("CREATED FILE",fp);
				fclose(fp);
				sprintf(buffer,"Config file not found, creating config file at device:/SmashscopeConfig.json");
			}else{
				sprintf(buffer,"Config file not found, Unable to create file");
			}
		}
	}
	else{
		sprintf(buffer,"fatInitDefault() failure");
	}
	*/
