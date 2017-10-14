#include <msp430.h>

#define LED_0 BIT0 //rename variables for easier coding
#define LED_1 BIT7
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT1
#define BUTTON2 BIT2
unsigned int blink = 0;
unsigned int blink2 = 0;
void main(void)
{
WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;//disable high impedance mode
LED_DIR |= LED_0; // Set P1.0 and P9.7 to outputs
P9DIR |= LED_1;
LED_OUT &= ~(LED_0); // Set the LEDs off
P9OUT &= ~LED_1;
P1REN |= BUTTON; //Enables pullup or pulldown resistor
P1OUT |= BUTTON; //pullup resistor selected
P1IE |= BUTTON; //enable interrupt for pin 1.1
P1REN |= BUTTON2; //Enables pullup or pulldown resistor
P1OUT |= BUTTON2; //pullup resistor selected
P1IE |= BUTTON2; //enable interrupt for pin 1.1

__enable_interrupt(); // interrupts are enabled
for (;;)//infinite loop
{
if(blink > 0) //activated when the button is pressed
{
P1OUT ^= (LED_0); // Toggle P1.0


__delay_cycles(100000); // sets delay
}

if(blink2 > 0) //activated when the button is pressed
{
// Toggle P9.7
P9OUT ^= (LED_1);

__delay_cycles(200000); // sets delay
}
}
}
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR //start interrupt
__interrupt void Port_1(void)
{
    if(P1IFG & BUTTON){
blink ^= 0x01; //toggle blink to 1, goes to for loop
P1IFG &= ~BUTTON; // clear flag
LED_OUT &= ~(LED_0); // turn LEDs off

P1IES ^= BUTTON; //toggle flag to rising edge
    }
    if(P1IFG & BUTTON2){

blink2 ^= 0x01; //toggle blink to 1, goes to for loop
P1IFG &= ~BUTTON2; // clear flag
P9OUT &= ~(LED_1);

P1IES ^= BUTTON2; //toggle flag to rising edge
}
}
//Since the flag is set to rising edge, when the button is released:
// - the flag is set to 1, triggering the interrupt again
// - blink = 0
// - the flag is turned off
// - LEDs are turned off
// - flag is set to rising edge
// - interrupt will not be triggered again until the button is pressed
