#pragma once
struct vertex{
    int8_t x;
    int8_t y;
};
struct goomwaveData{
    	bool firstWrite;
    	uint8_t modded_buttons[7];
    	struct vertex mainStickUserNotches[12];
    	struct vertex subStickUserNotches[12];
    	uint8_t ultimateMode;
	uint8_t PODE_RATE;
	uint8_t leftYoshi;
    	uint8_t rightYoshi;
    	int8_t sensitivityNumber;
    	bool uptiltToggle;
  	bool rumble;
  	bool pivots;
    	bool pivotTilts;
    	bool dbooc;
    	bool safeMode;
    	bool notches;
    	bool TauntDisable;
	bool PodeToggle; 
	uint8_t cNotchMode;
};
enum Button {aBtn, bBtn, xBtn, yBtn, rBtn, lBtn, zBtn, leftBtn, upBtn, rightBtn, downBtn, startBtn};


