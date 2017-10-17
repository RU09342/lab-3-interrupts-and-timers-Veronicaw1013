#include <msp430.h>

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;//disable high impedance mode
    P1DIR |= BIT0;   // pins 1.0 and 9.7 are set as outputs
    P9DIR |= BIT7;
    P1REN|= BIT1;   // pullup or pulldown resistor enabled
    P1OUT|= BIT1;   // pullup resistor selected
    P1IE |= BIT1;   // interrupt enable on port 1.1
    P1IES |= BIT1;  // set interrupt to falling edge
    P1IFG &= ~BIT1; // clear interrupt flag
    TA0CCTL0 = CCIE;    // capture/compare interrupt enabled
    TA0CCR0 = 3277;     // ACLK 32768/10Hz = about 3277
    TA0CTL= TASSEL_1+MC_1;  // Set timerA0 to ACLK, up mode
    __bis_SR_register(LPM0_bits + GIE); // enter LPM0 mode and enable global interrupts

}

#pragma vector= TIMER0_A0_VECTOR // TimerA0 interrupt
__interrupt void Timer_A0 (void)
    {
        P1OUT ^= BIT0;  // toggle first LED
    }


#pragma vector=PORT1_VECTOR     // button interrupt

    __interrupt void PORT_1(void)

    {

        if (P1IES & BIT1) // if button is pressed...

        {

            TA1CTL = TASSEL_1 + MC_2;   // timerA1 is set to ACLK, continuous mode

            P9OUT ^= BIT7;              // toggle second LED

            P1IES &= ~BIT1;             // set interrupt to rising edge

        }

        else                // if button is depressed...

        {

            TA0CCR0 = TA1R; // set CCR0 to the value counted in TA1R

            P9OUT &= ~BIT7; // turn off the second LED

            P1IES |= BIT1; // set interrupt to falling edge

            TA1CTL = TACLR; // clear timerA1

        }



        P1IFG &= ~BIT1; // clear interrupt flag

}

