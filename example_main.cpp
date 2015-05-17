#include "msp.h"
#include "MyDriverLib/mydriverlib.h"
#include "PID_v1.h"

//	Define Variables we'll be connecting to
double Setpoint, Input, Output;
//	Time Keeping Elements
uint32_t lastTime = 0;
//	Range Value for the PWM signal
uint32_t maxRange = 0;

//	PWM Frequency
uint32_t pwmFreq = 100000;	// 100kHz

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 2, 20, .01, DIRECT);

// ISR ADC - Assigns the value read to the Input Variable
extern "C" void ADC_ISR(){
	// Upon Successful Conversion - send data to Input
	Input = ADC14MEM0;
	myPID.Compute();
	set_TimerA0_CCR1(Output);
}

void main(void){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    //	Configure Hardware
    setup_clock();
    setup_TimerA0();	// Operates at 3MHz

    //	Configure the max range for the PWM signal
    maxRange = pwmFreqTicksCalc(pwmFreq, 3000000);

    //	Set the range in the PID Controller
    myPID.SetOutputLimits(0, maxRange-1);

    __enable_interrupts();

    //	Infinite Loop - SW starts here
    while(1){
    	// Should there be a poll of the port every Sample Time
		uint32_t sampleTime = myPID.GetSampleTime();
		uint32_t now = millis();
		uint32_t timeChange = (now - lastTime);

		if(timeChange >= sampleTime){
			start_ADC();	// Grab the value on port 5.5
		}
    }

}
