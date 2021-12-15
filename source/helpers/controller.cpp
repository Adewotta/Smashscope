#include <grrlib.h>
#include "controller.h"
int8_t padData[4][6];
int8_t prevPadData[4][6];
int buttonsHeld[4];
uint16_t buttonState[4];
uint16_t prevButtonState[4];
/*
 * Copy all current analog stick data
 * */
void updateController(){
	for(int i = 0; i < 4; i++){
		memcpy(prevPadData[i],padData[i],6);
		memcpy(prevButtonState,buttonState,8);
	}
	for(int i = 0; i < 4; i++){
		padData[i][0] = PAD_StickX(i);
		padData[i][1] = PAD_StickY(i);
		padData[i][2] = PAD_SubStickX(i);
		padData[i][3] = PAD_SubStickY(i);
		padData[i][4] = PAD_TriggerL(i);
		padData[i][5] = PAD_TriggerR(i);
		buttonState[i] = PAD_ButtonsHeld(i);
		if( PAD_ButtonsHeld(i) & PAD_BUTTON_B){
			buttonsHeld[i] += 1;
		}
		else{
			buttonsHeld[i] = 0;
		}
	}
	return;
}

/*
 *Check if the analog stick has passed a certain value, rising or falling
 */
int passedValue(int pad, int stick, int value){
	
	if(padData[pad][stick] <= value && prevPadData[pad][stick] > value){
		return FALLING;
	} else if(padData[pad][stick] >= value && prevPadData[pad][stick] < value){
		return RISING;
	}
	return NONE;
}
int passedValueAll(int stick,int value){

	for(int i = 0; i < 4; i++){
		int passed = passedValue(i,stick,value);
		if(passed!=NONE){
			return passed;
		}
	}
	return NONE;
}
bool exitPressed(){
	for(int i = 0; i < 4; i++){
		if(buttonsHeld[i] == 60){
			buttonsHeld[i]=61;
			return true;
		}
	}
	return false;
}

uint16_t PAD_ButtonsDownAll(){
	uint16_t padState = 0;
	for(int i = 0; i < 4; i++){ 
		padState |= buttonState[i] & (buttonState[i] ^ prevButtonState[i]);	
	}
	return padState;
}

uint16_t PAD_ButtonsHeldAll(){
	uint16_t padState = 0;
	for(int i = 0; i < 4; i++){
		padState |= buttonState[i];
	}
	return padState;
}



