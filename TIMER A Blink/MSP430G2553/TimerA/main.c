#include <msp430.h>
double convert(double); //converts a frequency in Hz to ticks used in ccr register
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR =BIT0|BIT6;
    TA0CTL=TASSEL_1+MC_1;
    TA0CCTL0 = 0x10;
    TA0CCR0=convert(10);
    _BIS_SR(LPM0_bits + GIE);
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT^=(BIT0|BIT6);
}
//converts frequency to ticks
double convert(double frequency){
    return 32000/frequency;
}
