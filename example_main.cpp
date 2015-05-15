#include "msp.h"
#include "MyDriverLib/mydriverlib.h"
#include "PID_v1.h"

void main(void){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    setup_clock();
    setup_TimerA0();

    __enable_interrupts();

    while(1);
}
