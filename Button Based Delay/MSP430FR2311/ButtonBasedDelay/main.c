#include <msp430.h>

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;//disable high impedance mode
    P1DIR |= BIT0;
    P2DIR |=  BIT0;   // pins 1.0 and 2.0 are set as outputs
    P1REN|= BIT1;   // pullup or pulldown resistor enabled
    P1OUT|= BIT1;   // pullup resistor selected
    P1IE |= BIT1;   // interrupt enable on port 1.1
    P1IES |= BIT1;  // set interrupt to falling edge
    P1IFG &= ~BIT1; // clear interrupt flag
    TB0CCTL0 = CCIE;    // capture/compare interrupt enabled
    TB0CCR0 = 3277;     // ACLK 32768/10Hz = about 3277
    TB0CTL= TBSSEL_1+MC_1;  // Set timerB0 to ACLK, up mode
    __bis_SR_register(LPM0_bits + GIE); // enter LPM0 mode and enable global interrupts

}

#pragma vector= TIMER0_B0_VECTOR // TimerB0 interrupt
__interrupt void Timer_B0 (void)
    {
        P1OUT ^= BIT0;  // toggle first LED
    }


#pragma vector=PORT1_VECTOR     // button interrupt

    __interrupt void PORT_1(void)

    {

        if (P1IES & BIT1) // if button is pressed...

        {

            TB1CTL = TBSSEL_1 + MC_2;   // timerB1 is set to ACLK, continuous mode

            P2OUT ^= BIT0;              // toggle second LED

            P1IES &= ~BIT1;             // set interrupt to rising edge

        }

        else                // if button is depressed...

        {

            TB0CCR0 = TB1R; // set CCR0 to the value counted in TB1R

            P2OUT &= ~BIT0; // turn off the second LED

            P1IES |= BIT1; // set interrupt to falling edge

            TB1CTL = TBCLR; // clear timerB1

        }



        P1IFG &= ~BIT1; // clear interrupt flag

}

