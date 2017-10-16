# TIMER A Blink
## Veronica Williams, October 15, 2017, Code Updated: October 14, 2017 
Controls the speed of two LEDs blinking with the TIMER module.


## Libraries for the MSP430
Msp430.h is a general header file that includes all the header files for boards in the MSP430 family. When creating a new project there is a pull down menu that will allow you to choose which board you are actually using. 

## General Format

A double integer "frequency" along with a constant double "ticks" were created to use as a frequency divider. The frequency divider will convert the desired hertz into a value that can be used to operate the TIMER module. 

double frequency = 10; // Frequency divider

const double ticks = 32768/frequency; // convert frequency to usable CCR0 value

The watchdog timer must be turned off for the five boards, and all of outputs were declared with their proper PxDIR ("0" sets an input and "1" sets an output) values. PxREN (pullup/pulldown resistor enabled) and PxIE (interrupt enable) also had to be used as the button will be used for the interrupts. The inputs and outputs for each board can be seen later. 

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

P1DIR =BIT0|BIT6; // set bits 1.0 and 1.6 as outputs

For the MSP430FR2311, MSP430FR5994, and MSP430FR6989, the GPIO power-on default high-impedance mode must also be disabled within the main function.

PM5CTL0 &= ~LOCKLPM5;

The timer also had to be declared and set so that it will function properly in the interrupt. Tx0CTL determines which clock source will be used as well as what mode will be used (up, up down, or continuous). Tx0CCTL0 enables interrupts for timer A. The value for the CCR0 register was set to "ticks" which was determined by the conversion stated previously. Timer A was used for all of the boards except for the MSP430FR2311, which uses Timer B (TB0CTL, TB0CCTL0, TB0CCR0).

TA0CTL=TASSEL_1+MC_1; //TA control, ACLK, up mode

TA0CCTL0 = CCIE; // enable interrupts for CCR0

TA0CCR0=ticks; // set value of TA0CCR0 to "ticks"

In order to enter Low Power Mode 0 and to enable general interrupts the next line must be added at the end of the main function.The low power mode can be changed depending on the developer's needs.
 
_BIS_SR(LPM0_bits + GIE);

Once the timer counts up to the value of CCR0 the timer interrupt will be flagged. Inside the interrupt the two LEDs will be toggled. In the case of the MSP430FR2311 the interrupt would change to TIMER0_B0_VECTOR and __interrupt void Timer_B(void)

#pragma vector = TIMER0_A0_VECTOR // timer A interrupt
__interrupt void Timer_A(void)
{
    P1OUT^=(BIT0|BIT6); // toggle LEDs
}

## Specific Code for Each Board
### MSP430G2553 where pins 1.6 and 1.0 are LEDs, pin 1.3 is a button-

Configure LEDs as outputs: P1DIR |= BIT6; P1DIR |= BIT0; 

Toggle LEDs: P1OUT ^= BIT6; P1OUT^=BIT0;

### MSP430FR6989 where pins 9.7 and 1.0 are LEDs, pin 1.1 is a button-

Configure LEDs as outputs: P9DIR |= BIT7; P1DIR |= BIT0;  

Toggle LEDs: P9OUT ^= BIT7; P1OUT^=BIT0;

### MSP430FR5994 where pin 1.1 and 1.0 are LEDs, pin 5.5 is a button-

Configure LEDs as outputs:  P1DIR |= BIT1; P1DIR |= BIT0;  

Toggle LEDs: P1OUT ^= BIT1; P1OUT^=BIT0;

### MSP430FR2311 where pin 1.0 and 2.0 are LEDs, pin 1.1 is a button-

Configure LEDs as outputs:  P1DIR |= 0x01; P2DIR |= BIT0; 

Toggle LEDs: P1OUT ^= 0x01; P2OUT^=BIT0;


### MSP430FR5529 where pin 1.0 and pin 4.7 are LEDs, pin 2.1 is a button-
Configure LEDs as outputs:  P1DIR |= 0x01;  P4DIR |= BIT7;  

Toggle LEDs: P1OUT ^= 0x01; P4OUT^=BIT7;

