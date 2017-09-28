#include <msp430.h>
#define LED1 BIT0
#define LED2 BIT6
#define BUTTON BIT3
volatile unsigned int i = 0;//to prevent optimization
void main(void)
{
WDTCTL=WDTPW | WDTHOLD;
P1DIR |= (LED1+LED2);//
P1OUT &= ~(LED1+LED2);
P1REN |= BUTTON;
P1IE |= BUTTON;
P1IFG &= ~BUTTON;
//__enable_interrupt();//enable all interrupts
_BIS_SR(LPM4_bits+GIE);
}

//port1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
P1OUT ^= (LED1+LED2);
P1IFG &= ~BUTTON;
P1IES ^= BUTTON;
}
