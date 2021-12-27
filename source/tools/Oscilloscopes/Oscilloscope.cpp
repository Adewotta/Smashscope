#include <stdint.h>
#include "grrlib.h"
#include <string>
#include <time.h>
#include "../../globals/fonts.h"
#include "../../helpers/controller.h"
#include "../../config/config.h"
#include "fat.h"
#include <stdio.h>
#include <string>
#define LEN 1000
#define ARR LEN + 4
int8_t analogData[6][ARR];
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
	float x1,x2,y1,y2;
	int ctr = std::max(0,counter - 1);
	for(int i = prevCounter; i !=ctr; i=((i+1)%LEN)){	
		x1 = i * (640.0/LEN);
		x2 = x1+1;;
		//Clear Vertical Strip
		GRRLIB_Rectangle(((int) (x1+5))%640,0,3,480,0xFFFFFFFF,true);
		GRRLIB_Rectangle(((int) (x1+3))%640,0,4,480,0x0A0A20FF,true);
		//Draw data
		for(int x = 0; x < 6; x++){
			y1 = 240 + analogData[x][i];
			y2 = 240 + analogData[x][(i+1)%LEN];
			GRRLIB_Line(x1,y1,x2,y2,analogColors[x]);
		}
	}
	prevCounter = ctr;	

}

//Get timestamp
void getTimeStamp(char* buf,int size){
	time_t sysTime;
	struct tm ts;
	time(&sysTime);
	ts = *localtime(&sysTime);
	strftime(buf, size, "%a%Y-%m-%d-%H-%M-%S", &ts);

}



void DataToString(char* buffer){
	char* ptr = buffer;
	ptr+= snprintf(ptr,100,"StickX,StickY,SubStickX,SubStickY,TriggerL,TriggerR\n");
	for(int i = 0; i < LEN; i++){
		int index = (i+counter)%LEN;
		int rowLen = snprintf(ptr,35,"%d,%d,%d,%d,%d,%d\n",analogData[0][index],analogData[1][index],analogData[2][index],analogData[3][index],analogData[4][index],analogData[5][index]);
		ptr+=rowLen;
	}
}

//Dump current data to CSV on SD card
void saveCSV(){
	
	//Open File
	static	char date[40];
	static char filename[50];
	getTimeStamp(date,40);
	sprintf(filename,"%s.csv",date);
	FILE *fp;	
	fp = fopen(filename, "w");

	char buf[20];
	sprintf(buf,"%d",(int)fp);
	GRRLIB_PrintfTTF (20,20, menuFont, filename,14, 0xFFFFFFFF);	
	GRRLIB_PrintfTTF (20,60, menuFont, buf,14, 0xFFFFFFFF);	

	if(fp != nullptr){//If file is available to write to
		char csvData[LEN*6*6];//Open ~80kb buffer for writing csv data
		DataToString(csvData);
		GRRLIB_Rectangle(0,0,640,480,0xFF0000FF,true);
		fprintf(fp,csvData);
		fclose(fp);
	}
}

void runOscilloscope(){
	SI_RegisterPollingHandler(&readPolls);
	char buffer[50];
	auto screen = GRRLIB_CreateEmptyTexture(640,480);	
	GRRLIB_Screen2Texture(0,0,screen,false);
	SI_SetSamplingRate(1);//Set to 1ms poll sped
	GRRLIB_SetAntiAliasing(false);
	SI_SetXY(8,33);
	while(1){
		PAD_ScanPads();
		updateController();
		
		GRRLIB_2dMode();
		GRRLIB_DrawImg(0,0,screen,0,1,1,0xffffffff);
		drawScope();
		GRRLIB_Screen2Texture(0,0,screen,false);


		if(PAD_ButtonsDownAll() & PAD_BUTTON_START){
			saveCSV();
		}
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



