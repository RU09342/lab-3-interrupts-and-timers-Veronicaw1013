#include <msp430.h>

#define LED_0 BIT0 //rename variables for easier coding
#define LED_1 BIT7
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT1
unsigned int blink = 0;
void main(void)
{
WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
LED_DIR |= LED_0; // Set P1.0 and P4.7 to outputs
P4DIR |= LED_1;
LED_OUT &= ~(LED_0); // Set the LEDs off
P4OUT &= ~LED_1;
P2REN |= BUTTON; //Enables pullup or pulldown resistor
P2OUT |= BUTTON; //pullup resistor selected
P2IE |= BUTTON; //enable interrupt for pin 5.6
__enable_interrupt(); // interrupts are enabled
for (;;)//infinite loop
{
if(blink > 0) //activated when the button is pressed
{
P1OUT ^= (LED_0 + LED_1); // Toggle P1.0 and P4.7
P4OUT ^= (LED_1);

__delay_cycles(100000); // sets delay
}
}
}
// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR //start interrupr
__interrupt void Port_2(void)
{
blink ^= 0x01; //toggle blink to 1, goes to for loop
P2IFG &= ~BUTTON; // clear flag
LED_OUT &= ~(LED_0); // turn LEDs off
P4OUT &= ~(LED_1);

P2IES ^= BUTTON; //toggle flag to rising edge
}
//Since the flag is set to rising edge, when the button is released:
// - the flag is set to 1, triggering the interrupt again
// - blink = 0
// - the flag is turned off
// - LEDs are turned off
// - flag is set to rising edge
// - interrupt will not be triggered again until the button is pressed
