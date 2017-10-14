# Button Interrupt
## Veronica Williams, October 14, 2017, Code Updated: October 11, 2017 
Two LEDs were toggled on the boards using a button interrupt. 


## Libraries for the MSP430
Msp430.h is a general header file that includes all the header files for boards in the MSP430 family. When creating a new project there is a pull down menu that will allow you to choose which board you are actually using. 

## General Format

The inputs and outputs for each board were defined and renamed for easier coding. The different cases for each board can be seen in the "Specific Code for Each Board" section.

#define LED_0 BIT0 //rename variables for easier coding

#define LED_1 BIT7

#define LED_OUT P1OUT

#define LED_DIR P1DIR

#define BUTTON BIT1

An integer "blink" was created and initialized as zero so it could be used within the for loops in the main section of the code. 

unsigned int blink = 0;

In the main function the watchdog timer must be turned off for the five boards, and all of the inputs and outputs were declared with their proper PxOUT (when I/O is configured to output mode: 0b = output is low, 1b = output is high). PxOUT can also set whether the resistor is a pullup (1) or pulldown (0) resistor and PxDIR ("0" sets an input and "1" sets an output) values. PxREN (pullup/pulldown resistor enabled) and PxIE (interrupt enable) also had to be used as the button will be used for the interrupts. The inputs and outputs for each board can be seen later. 

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

LED_DIR |= LED_0; // Set P1.0 and P4.7 to outputs

P4DIR |= LED_1;

LED_OUT &= ~(LED_0); // Set the LEDs off

P4OUT &= ~LED_1;

P2REN |= BUTTON; //Enables pullup or pulldown resistor

P2OUT |= BUTTON; //pullup resistor selected

P2IE |= BUTTON; //enable interrupt for pin 2.1

For the MSP430FR2311, MSP430FR5994, and MSP430FR6989, the GPIO power-on default high-impedance mode must also be disabled within the main function.

PM5CTL0 &= ~LOCKLPM5;

 
An infinite loop was also created so the code within the loop will run forever. 

for(;;)

{

if(blink > 0) //activated when the button is pressed

{

P1OUT ^= (LED_0); // Toggle P1.0 and P4.7

P4OUT ^= (LED_1);

__delay_cycles(100000); // sets delay

}

}

}

When blink is changed to "1" in the interrupt, this code will run. It toggles the LEDs at a specified rate. 

The interrupt is triggered when the button is pressed. Once it is pressed, blink is set to "1", the flag is cleared, the LEDs are turned off, and the interrupt is set to trigger on the rising edge. When the button is released, the flag bit is set to "1" again, triggering the interrupt. This time blink is set to "0", the flag bit is set to "0", the LEDs are turned off, and the trigger is set to falling edge again. The interrupt will not be triggered again until the button is pressed. 

#pragma vector=PORT2_VECTOR //start interrupt

__interrupt void Port_2(void)

{

blink ^= 0x01; //toggle blink to 1, goes to for loop

P2IFG &= ~BUTTON; // clear flag

LED_OUT &= ~(LED_0); // turn LEDs off

P4OUT &= ~(LED_1);

P2IES ^= BUTTON; //toggle flag to rising edge

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

## Extra Work
Two buttons were implemented on the MSP430FR6989. When the button connected to pin 1.0 is pressed, the red LED is toggled. When the button connected to pin 9.7 is pressed the green LED is toggled. 

