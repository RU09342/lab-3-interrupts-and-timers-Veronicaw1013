# Button Based Delay
## Veronica Williams, October 16, 2017, Code Updated: October 16, 2017 
Microcontroller will initially blink one LED at a rate of 10 Hz. Depending on how long the button is pressed, the microcontroller will set the length of that press to the new rate at which the LED blinks.  


## Libraries for the MSP430
Msp430.h is a general header file that includes all the header files for boards in the MSP430 family. When creating a new project there is a pull down menu that will allow you to choose which board you are actually using. 

## General Format

The watchdog timer must be turned off for the five boards, and all of outputs were declared with their proper PxDIR ("0" sets an input and "1" sets an output) values. PxREN (pullup/pulldown resistor enabled) and PxIE (interrupt enable), PxIES ("0" is rising edge, "1" is falling edge), and PxIFG ("0" clears interrupt flag, "1" sets the interrupt) also had to be used as the button will be used for the interrupts. The inputs and outputs for each board can be seen later. 

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

P1DIR |= BIT0 + BIT6;   // pins 1.0 and 1.6 are set as outputs

P1REN|= BIT3;   // pullup or pulldown resistor enabled

P1OUT|= BIT3;   // pullup resistor selected

P1IE |= BIT3;   // interrupt enable on port 1.3

P1IES |= BIT3;  // set interrupt to falling edge

P1IFG &= ~BIT3; // clear interrupt flag

For the MSP430FR2311, MSP430FR5994, and MSP430FR6989, the GPIO power-on default high-impedance mode must also be disabled within the main function.

PM5CTL0 &= ~LOCKLPM5;

The timer also had to be declared and set so that it will function properly in the interrupt. Tx0CTL determines which clock source will be used as well as what mode will be used (up, up down, or continuous). Tx0CCTL0 enables interrupts for timer A. The value for the CCR0 register was set to 3277, which is the clock source divided by 10 Hz. Timer A was used for all of the boards except for the MSP430FR2311, which uses Timer B (TB0CTL, TB0CCTL0, TB0CCR0).

TA0CCTL0 = CCIE;    // capture/compare interrupt enabled

TA0CCR0 = 3277;     // ACLK 32768/10Hz = about 3277

TA0CTL= TASSEL_1+MC_1;  // Set timerA0 to ACLK, up mode

In order to enter Low Power Mode 0 and to enable general interrupts the next line must be added at the end of the main function.The low power mode can be changed depending on the developer's needs.
 
_BIS_SR(LPM0_bits + GIE);

Once the timer counts up to the value of CCR0 the timer interrupt will be flagged. Inside the interrupt the first LED will be toggled. In the case of the MSP430FR2311 the interrupt would change to TIMER0_B0_VECTOR and __interrupt void Timer_B(void)

#pragma vector= TIMER0_A0_VECTOR // TimerA0 interrupt
__interrupt void Timer_A0 (void)
    {
        P1OUT ^= BIT0;  // toggle first LED
    }
    
Another interrupt occurs when the button is pressed. 

#pragma vector=PORT1_VECTOR     // button interrupt

   __interrupt void PORT_1(void)
  
Inside of the interrupt is a if-else statement to determine whether the button is being pressed or depressed. If the button is pressed, the timer A1 clock will be set to ACLK and in continuous mode, and it starts counting. The second LED is also turned on to show that the interrupt has occurred. The interrupt is then set to trigger on the rising edge, or when the button is released. The interrupt flag is cleared. 

 {

    if (P1IES & BIT3) // if button is pressed...

     {

        TA1CTL = TASSEL_1 + MC_2;   // timerA1 is set to ACLK, continuous mode

        P1OUT ^= BIT6;              // toggle second LED

        P1IES &= ~BIT3;             // set interrupt to rising edge

        }
        
When the button is released, CCR0 is set to the value that timer A1 counted to, thus setting a new value where the timer A0 interrupt will occur, and the LED will toggle. The second LED is also turned off to show that the interrupt flag is no longer set. The interrupt is then set back to the falling edge, timer A1 and the interrupt flag are cleared in order to be ready for the next button press. 

else                // if button is depressed...

   {

      TA0CCR0 = TA1R; // set CCR0 to the value counted in TA1R

      P1OUT &= ~BIT6; // turn off the second LED

      P1IES |= BIT3; // set interrupt to falling edge

      TA1CTL = TACLR; // clear timerA1

   }

## Specific Code for Each Board
### MSP430G2553 where pins 1.6 and 1.0 are LEDs, pin 1.3 is a button-

Configure LEDs as outputs: P1DIR |= BIT6; P1DIR |= BIT0; 

Start with LEDs off: P1OUT &= !(BIT0 and BIT6);

Configure button as input: P1DIR &= ~BIT3; P1REN |= BIT3; P1OUT |= BIT3;

Configure button to interrupt: P1IE |= BIT3; P1IFG &= ~BIT3; P1IES ^= BIT3; (To flip interrupt to rising or falling edge)

Toggle LEDs: P1OUT ^= BIT6; P1OUT^=BIT0;

### MSP430FR6989 where pins 9.7 and 1.0 are LEDs, pin 1.1 is a button-

Configure LEDs as outputs: P9DIR |= BIT7; P1DIR |= BIT0;  

Start with LEDs off: P1OUT &= !(BIT0); P9OUT &= !(BIT0);

Configure button as input: P1DIR &= ~BIT1; P1REN |= BIT1; P1OUT |= BIT1;

Configure button to interrupt: P1IE |= BIT1; P1IFG &= ~BIT1; P1IES ^= BIT1; (To flip interrupt to rising or falling edge)

Toggle LEDs: P9OUT ^= BIT7; P1OUT^=BIT0;

### MSP430FR5994 where pin 1.1 and 1.0 are LEDs, pin 5.5 is a button-

Configure LEDs as outputs:  P1DIR |= BIT1; P1DIR |= BIT0;  

Start with LEDs off: P1OUT &= !(BIT1 and BIT0);

Configure button as input: P5DIR &= ~BIT5; P5REN |= BIT5; P5OUT |= BIT5;

Configure button to interrupt: P5IE |= BIT6; P5IFG &= ~BIT6; P5IES ^= BIT6; (To flip interrupt to rising or falling edge)

Toggle LEDs: P1OUT ^= BIT1; P1OUT^=BIT0;

### MSP430FR2311 where pin 1.0 and 2.0 are LEDs, pin 1.1 is a button-

Configure LEDs as outputs:  P1DIR |= 0x01; P2DIR |= BIT0; 

Start with LEDs off: P1OUT &= !(BIT0); P2OUT &= !(BIT0);

Configure button as input: P1DIR &= ~BIT1; P1REN |= BIT1; P1OUT |= BIT1;

Configure button to interrupt: P1IE |= BIT1; P1IFG &= ~BIT1; P1IES ^= BIT1; (To flip interrupt to rising or falling edge)

Toggle LEDs: P1OUT ^= 0x01; P2OUT^=BIT0;

### MSP430FR5529 where pin 1.0 and pin 4.7 are LEDs, pin 2.1 is a button-
Configure LEDs as outputs:  P1DIR |= 0x01;  P4DIR |= BIT7;  

Start with LEDs off: P1OUT &= !(BIT0); P4OUT &= !(BIT7);

Configure button as input: P2DIR &= ~BIT1; P2REN |= BIT1; P2OUT |= BIT1;

Configure button to interrupt: P2IE |= BIT1; P2IFG &= ~BIT1; P2IES ^= BIT1; (To flip interrupt to rising or falling edge)

Toggle LEDs: P1OUT ^= 0x01; P4OUT^=BIT7;
