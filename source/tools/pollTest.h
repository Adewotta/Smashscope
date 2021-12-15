#pragma once
#include <stdint.h>
#include "grrlib.h"
#include "ogc/lwp_watchdog.h"
#include <string>
#include "../globals/fonts.h"
#include "../helpers/controller.h"

typedef union {
	uint32_t data;
	struct{
		int8_t x;
		int8_t y;
		int8_t cx;
		int8_t cy;
	};
} Sticks;
void runPollTest();
