#include "mydriverlib.h"

/*
 * Variables Used
 */
volatile uint32_t millisecond_count = 0;

/*
 * Sets up the TimerA0 Module to Run on 3MHz
 * Assigns -
 * 	CCR0 to fire an interrupt every millisecond
 */
void setup_TimerA0(){
	// Configure Timer0A to operate in Up count mode with 3MHz
	TA0CTL = TASSEL__SMCLK | ID__8 | MC__UP;	// Source select, divider, count mode
	TA0EX0 = TAIDEX__2;	// Input Divider
	TA0CTL |= TACLR;	// To reset TA0R and apply dividers (Auto resets)

	// Configure CCR0 to fire an Interrupt every 1ms for millis()
	TA0CCTL0 = CCIE;		// Interrupt enable
	NVIC_ISER0 = 1 << ((INT_TA0_0 - 16) & 31);	//	Add to NVIC

	// CCR0 Configuration
	TA0CCR0 = ticks_1ms;	// 1ms of time per fire

	// CCR1 Configuration for PWM with Output Port
	TA0CCTL1 = OUTMOD_6;
	P2DIR |= BIT4;	//
	P2SEL0 |= BIT4;	//	Enable Output On P2.4
	P2SEL1 &= ~BIT4;//
}

/*
 * Configure CCR1's Value
 */
void set_TimerA0_CCR1(uint32_t ticks){
	TA0CCR1 = ticks;
}

/*
 * Sets up the DCO with 48MHz
 * Assigns -
 * 	MCLK = DCO 48MHz
 *	SMCLK = DCO 48MHz
 */
void setup_clock() {
	uint32_t currentPowerState;

	// NOTE: This assumes the default power state is AM0_LDO.
	//	5.2 In AM0_LDO DCO is selected for MCLK, HSMCLK, SMCLK by default

	/* Step 1: Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */

	/* Get current power state, if it's not AM0_LDO, error out */
	currentPowerState = PCMCTL0 & CPM_M;
	if (currentPowerState != CPM_0)
		error();

	while ((PCMCTL1 & PMR_BUSY));
	PCMCTL0 = CS_KEY<<16 | AMR_1;
	while ((PCMCTL1 & PMR_BUSY));
	if (PCMIFG & AM_INVALID_TR_IFG)
		error();                            // Error if transition was not successful
	if ((PCMCTL0 & CPM_M) != CPM_1)
		error();                            // Error if device is not in AM1_LDO mode

	/* Step 2: Configure Flash wait-state to 2 for both banks 0 & 1 */
	FLCTL_BANK0_RDCTL = (FLCTL_BANK0_RDCTL & ~FLCTL_BANK0_RDCTL_WAIT_M) | FLCTL_BANK0_RDCTL_WAIT_2;
	FLCTL_BANK1_RDCTL = (FLCTL_BANK0_RDCTL & ~FLCTL_BANK1_RDCTL_WAIT_M) | FLCTL_BANK1_RDCTL_WAIT_2;

	/* Step 3: Configure DCO to 48MHz */
	CSKEY = CS_KEY;                        // Unlock CS module for register access
	CSCTL0 = 0;                            // Reset tuning parameters
	CSCTL0 = DCORSEL_5;                    // Set DCO to 48MHz

	/* Step 4: Configure CS Sources and Dividers	 */
	/* Map MCLK = DCO, no divider (48MHz) */
	CSCTL1 &= ~(SELM_M | DIVM_M);	// Clear the clock Src and Divider
	CSCTL1 |= SELM__DCOCLK;			// Map DCO -> MCLK

	/* Map SMCLK = DCO / 16 (3Mhz) */
//	CSCTL1 &= ~(SELM_M | DIVM_M);		// Clear the clock Src and Divider
//	CSCTL1 |= SELS__DCOCLK | DIVS__16;	// Map DCO/16 -> MCLK

	// Lock CS module from unintended accesses
	CSKEY = 0;

    /* Step 5: Output MCLK to port pin to demonstrate 48MHz operation */
//    P4DIR |= BIT3;
//    P4SEL0 |=BIT3;                         // Output MCLK
//    P4SEL1 &= ~(BIT3);
}

//	Set up the ADC for 10 Bit operation
//	With Continous Samples on the same channel
void setup_ADC(){
	ADC14CTL0 &= ~(ADC14ENC);	// Allow configuration fo the ADC14 Module
	//			Use mclk , single ch conv,  signal from sample timer
	ADC14CTL0 = ADC14SSEL__MCLK |	ADC14CONSEQ_0 | ADC14SHP | ADC14ON ;
	ADC14CTL1 = ADC14RES__10BIT;	// Mod to get speed and resolution required!

	// Set up INPUT channel source for ADC14MEM
	ADC14MCTL0 = ADC14INCH_0;

	// Set up A0 Pin on Board (P5.5)
	P5SEL0 |= BIT5;
	P5SEL1 |= BIT5;

	// Set up Interrupt for Conversion Success
	NVIC_ISER0 = 1 << ((INT_ADC14 - 16) & 31);// Enable ADC interrupt in NVIC module
	ADC14IER0 = ADC14IE0;                     // Enable ADC14IFG.0
}

/*
 * Calculates the number ticks required to generate a desired
 * number of ticks needed to achieve frequency
 * Note assumes that Operating Frequency is greater than Target
 */
uint32_t pwmFreqTicksCalc(uint32_t target_freq, uint32_t operating_freq){
	double period_desired = (double)1/target_freq;
	double period_operating = (double)1/operating_freq;
	return period_desired/period_operating;
}

//	Start the Sampling and Conversion from P5.5
void start_ADC(){
	//Start Conversions from the selected channel
	ADC14CTL0 |= ADC14ENC | ADC14SC;        // Start conversion -software trigger
}

// API Function Millis() - Returns milliseconds elapsed
uint32_t millis(){
	return millisecond_count;
}

// ISR Millisecond incrementor
extern "C" void timerA0_ISR(void){
	TA0CCTL0 &= ~CCIFG;	// Clear the Interrupt
	millisecond_count++;
	TA0CCR0 += ticks_1ms;
}

/*
 * Error Function that Halts System Operation
 * Debugging should reveal which function was called from
 * stack image.
 */
void error(void)
{
    volatile uint32_t i;
    P1DIR |= BIT0;
    while (1)
    {
        P1OUT ^= BIT0;
        __delay_cycles(3100000);
    }
}

