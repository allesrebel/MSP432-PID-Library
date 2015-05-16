#include "msp.h"
#include "MyDriverLib/mydriverlib.h"
#include "PID_v1.h"

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void main(void){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    setup_clock();
    setup_TimerA0();

    __enable_interrupts();


    Input = 0;
    Setpoint = 100;

    myPID.SetMode(AUTOMATIC);

    while(1){
    	Input = 0;
    	myPID.Compute();
    }
}
