#pragma once
#include <stdint.h>
enum{STICKX,STICKY,SUB_STICKX,SUB_STICKY,TRIGGER_L,TRIGGER_R};
enum{NONE,RISING,FALLING};
uint16_t PAD_ButtonsDownAll();
uint16_t PAD_ButtonsHeldAll();
int passedValueAll(int stick,int value);
void updateController();
bool exitPressed();
int passedValue(int pad, int stick, int value);
