#include <msp430.h>

void main(void)
{
    double frequency = 10; // Frequency divider
    const double ticks = 32768/frequency; // convert frequency to usable CCR0 value
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer
    P1DIR |= BIT0; // set bits 1.0 and 2.0 as outputs
    P2DIR |= BIT0;
    TB0CTL=TBSSEL_1+MC_1; //TB control, ACLK, up mode
    TB0CCTL0 = CCIE; // enable interrupts for CCR0
    TB0CCR0=ticks; // set value of TB0CCR0 to "ticks"
    _BIS_SR(LPM0_bits + GIE); // enter LPM0 mode and enable global interrupts
}
#pragma vector = TIMER0_B0_VECTOR // timer B interrupt
__interrupt void Timer_B(void)
{
    P1OUT ^= BIT0; // toggle LEDs
    P2OUT ^= BIT0;
}