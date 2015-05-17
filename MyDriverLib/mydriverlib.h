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
//	Useful to generate number of
uint32_t pwmFreqTicksCalc(uint32_t target_freq, uint32_t operating_freq);
//---------------------------------------------------------

//---------------------------------------------------------
//	Library Init Functions
//---------------------------------------------------------

//	Set up Clocks MCLK, SMCLK, and ACLK to 48MHz
void setup_clock();

//	Set up the Timer0 to be operating on 6MHz w/ 1ms tick
void setup_TimerA0();

//	Set up the ADC to do fastest poll on channel 1
void setup_ADC();
//---------------------------------------------------------

//---------------------------------------------------------
//	API Functions
//---------------------------------------------------------
uint32_t millis();
void start_ADC();	//	Sample/convert Channel 0 (P5.5)
void set_TimerA0_CCR1(uint32_t ticks);
//---------------------------------------------------------

#endif
