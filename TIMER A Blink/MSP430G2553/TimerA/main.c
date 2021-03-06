#include <msp430.h>

void main(void)
{
    double frequency = 10; // Frequency divider
    const double ticks = 32768/frequency; // convert frequency to usable CCR0 value
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer
    P1DIR =BIT0|BIT6; // set bits 1.0 and 1.6 as outputs
    TA0CTL=TASSEL_1+MC_1; //TA control, ACLK, up mode
    TA0CCTL0 = CCIE; // enable interrupts for CCR0
    TA0CCR0=ticks; // set value of TA0CCR0 to "ticks"
    _BIS_SR(LPM0_bits + GIE); // enter LPM0 mode and enable global interrupts
}
#pragma vector = TIMER0_A0_VECTOR // timer A interrupt
__interrupt void Timer_A(void)
{
    P1OUT^=(BIT0|BIT6); // toggle LEDs
}
