#include <stdint.h>
#include "grrlib.h"
#include "ogc/lwp_watchdog.h"
#include "pollTest.h"
#include <string>
#include "../globals/fonts.h"
#include "../helpers/controller.h"

int pollSpeed = 0;
int timer = 0;
float averageUpdate = 0;
bool pause = false;
float timeBetweenPolls = 0;
uint64_t timedPoll = 16;
Sticks prevSticks;
Sticks sticks;

/*
 *Load sticks into a union, and compare them to see if any analog changes are recorded
 */
void readPolls(uint32_t a,void* b){
	timeBetweenPolls = diff_usec(timedPoll,gettime())/1000.0;
	timedPoll = gettime();

	sticks.x = PAD_StickX(0);
	sticks.y = PAD_StickY(0);
	
	sticks.cx = PAD_SubStickX(0);
	sticks.cy = PAD_SubStickY(0);
	if(prevSticks.data == sticks.data){
		timer++;
	}
	else{
		if(pause==false){
			//Give weight to the current value, and average it with the new value
			averageUpdate = (averageUpdate * 50.0 + timer) / 51.00; 
			timer = 0;    
			prevSticks.data = sticks.data;
	    	}
	}
}

std::string boolToString(bool a){
	if(a){
		return "Paused";
	}
	else{
		return "Not Paused";
	}
}

void runPollTest(){
	SI_RegisterPollingHandler(readPolls);
	SI_SetSamplingRate(1);//Set to 1ms poll sped
	while(1){
		PAD_ScanPads();
		updateController();
		GRRLIB_2dMode();
		char buffer[50];
			
		sprintf(buffer,"Polling Frequency: %.03fhz", 1000/timeBetweenPolls);
		GRRLIB_PrintfTTF (100, 100, menuFont, buffer, 16, 0xFFFFFFFF);	

		sprintf(buffer,"Average Polls Between Updates: %.03f", averageUpdate);
		GRRLIB_PrintfTTF (100, 120, menuFont, buffer, 16, 0xFFFFFFFF);
	
		sprintf(buffer,"Time Between Updates: %.03fms", averageUpdate*timeBetweenPolls);
		GRRLIB_PrintfTTF (100, 140, menuFont, buffer, 16, 0xFFFFFFFF);

		sprintf(buffer,"Time Between Polls: %.03fms", timeBetweenPolls);
		GRRLIB_PrintfTTF (100, 160, menuFont, buffer, 16, 0xFFFFFFFF);

		sprintf(buffer,boolToString(pause).c_str());
		GRRLIB_PrintfTTF (100, 80, menuFont, buffer, 16, 0xFFFFFFFF);

		if(PAD_ButtonsDownAll() & PAD_BUTTON_A){
			pause=!pause;
		}
		if(exitPressed()){
			break;
		}
		GRRLIB_Render();
    	}	
	SI_UnregisterPollingHandler(readPolls);
	return;
}
