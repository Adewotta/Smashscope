#include <stdint.h>
#include "grrlib.h"
#include <string>

#include "../../globals/fonts.h"
#include "../../helpers/controller.h"
#include "../../config/config.h"

#define LEN 1000
#define ARR LEN + 4
int8_t analogData[6][LEN];
static int counter = 2;
static bool pause = false;
static void readPolls(uint32_t a,void* b){

	PAD_ScanPads();
	if(pause==false){	
		analogData[0][counter] = PAD_StickX(0);
		analogData[1][counter] = PAD_StickY(0);
		analogData[2][counter] = PAD_SubStickX(0);
		analogData[3][counter] = PAD_SubStickY(0);
		analogData[4][counter] = PAD_TriggerL(0)-128;
		analogData[5][counter] = PAD_TriggerR(0)-128;
		counter++;
		if(counter >= LEN){
			counter = 0;
		}
	}	
}

static std::string boolToString(bool a){
	if(a){
		return "Paused";
	}
	else{
		return "Not Paused";
	}
}


const uint32_t analogColors[6] = {0xFF000080,0x0000FF80,0x00FF00FF,0xFFFF00FF,0xFF00FFFF,0xFFA000FF};


void drawScope(){
	static int prevCounter = 0;

	GRRLIB_Line(0,240+128,640,240+128,0xffffffff);//Upper Value line
	GRRLIB_Line(0,240,640,240,0xffffffff);//Center line
	GRRLIB_Line(0,240-128,640,240-128,0xffffffff);//Lower Value line

	int inc = 1;
	float x1,x2,y1,y2;
	int ctr = counter-1;
	for(int i = prevCounter; i !=ctr; i=((i+1)%LEN)){	
		x1 = i * (645.0/LEN);
		x2 = x1+1;
		//Clear Vertical Strip
		GRRLIB_Rectangle(((int) (x1+5))%640,0,3,480,0xFFFFFFFF,true);
		GRRLIB_Rectangle(((int) (x1+3))%640,0,4,480,0x0A0A20FF,true);
		//Draw data
		for(int x = 0; x < 6; x++){
			y1 = 240 + analogData[x][i];
			y2 = 240 + analogData[x][i+inc];
			//Draw data repeatedly to give "Bolder" Lines
			for(float i = 0; i < 2; i+=.5){
				GRRLIB_Line(x1+i,y1,x2+i,y2,analogColors[x]);
			}
		}
	}
	prevCounter = ctr;	

}


void runOscilloscope(){
	SI_RegisterPollingHandler(&readPolls);
	char buffer[50];
	auto screen = GRRLIB_CreateEmptyTexture(640,480);	
	GRRLIB_Screen2Texture(0,0,screen,false);
	SI_SetSamplingRate(1);//Set to 1ms poll sped
	GRRLIB_SetAntiAliasing(false);
	while(1){
		SI_SetXY(8,33);
		PAD_ScanPads();
		updateController();
		GRRLIB_2dMode();
		GRRLIB_DrawImg(0,0,screen,0,1,1,0xffffffff);
		drawScope();
		GRRLIB_Screen2Texture(0,0,screen,false);
		if(PAD_ButtonsDownAll() & PAD_BUTTON_A){
			pause=!pause;
		}
		if(exitPressed()){
			break;
		}
	
		GRRLIB_Render();
    	}	
	GRRLIB_FreeTexture(screen);
	SI_UnregisterPollingHandler(readPolls);
	return;
}



