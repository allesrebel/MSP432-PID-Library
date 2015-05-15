#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#include "msp.h"

//---------------------------------------------------------
//	Defines for Hardware Configuration
//---------------------------------------------------------
// !TODO: Remove this definition once the ti header file is updated this def.
#define CS_KEY 0x695A
#define FLCTL_BANK0_RDCTL_WAIT__2    (2 << 12)
#define FLCTL_BANK1_RDCTL_WAIT__2    (2 << 12)

//	Defines for Internal Use
#define ticks_1ms 3000
//---------------------------------------------------------

//---------------------------------------------------------
//	Variables for Internal use
//---------------------------------------------------------
extern volatile uint32_t millisecond_count;
//---------------------------------------------------------

//---------------------------------------------------------
//	Internal Utility Functions
//---------------------------------------------------------
//	Error Function
void error();
//---------------------------------------------------------

//---------------------------------------------------------
//	Library Init Functions
//---------------------------------------------------------

//	Set up Clocks MCLK, SMCLK, and ACLK to 48MHz
void setup_clock();

//	Set up the Timer0 to be operating on 6MHz w/ 1ms tick
void setup_TimerA0();
//---------------------------------------------------------

//---------------------------------------------------------
//	API Functions
//---------------------------------------------------------
uint32_t millis();
//---------------------------------------------------------

#endif
