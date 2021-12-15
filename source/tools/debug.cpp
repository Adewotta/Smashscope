#include <stdint.h>
#include "grrlib.h"
#include "ogc/lwp_watchdog.h"
#include "pollTest.h"
#include <string>
#include "../globals/fonts.h"
#include "../helpers/controller.h"
#include "../config/config.h"
void runDebugInfo(){
	while(1){
		PAD_ScanPads();
		updateController();
		GRRLIB_2dMode();

		GRRLIB_PrintfTTF (60,55, menuFont,"Dev Build",12, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (60,70, menuFont, ("JSON DUMP: " + getConfigString()).c_str(),12, 0xFFFFFFFF);
		GRRLIB_PrintfTTF (60,85, menuFont,("CONFIG ERROR CODE: " + getCode()).c_str(),12, 0xFFFFFFFF);

		if(exitPressed()){
			break;
		}
		GRRLIB_Render();
    	}	
	return;
}
