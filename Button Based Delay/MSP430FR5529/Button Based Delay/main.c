#include <msp430.h>

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer
    P1DIR |= BIT0;
    P4DIR |= BIT7;  // pins 1.0 and 4.7 are set as outputs
    P2REN|= BIT1;   // pullup or pulldown resistor enabled
    P2OUT|= BIT1;   // pullup resistor selected
    P2IE |= BIT1;   // interrupt enable on port 2.1
    P2IES |= BIT1;  // set interrupt to falling edge
    P2IFG &= ~BIT1; // clear interrupt flag
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


#pragma vector=PORT2_VECTOR     // button interrupt

    __interrupt void PORT_2(void)

    {

        if (P2IES & BIT1) // if button is pressed...

        {

            TA1CTL = TASSEL_1 + MC_2;   // timerA1 is set to ACLK, continuous mode

            P4OUT^= BIT7;              // toggle second LED

            P2IES &= ~BIT1;             // set interrupt to rising edge

        }

        else                // if button is depressed...

        {

            TA0CCR0 = TA1R; // set CCR0 to the value counted in TA1R

            P4OUT &= ~BIT7; // turn off the second LED

            P2IES |= BIT1; // set interrupt to falling edge

            TA1CTL = TACLR; // clear timerA1

        }



        P2IFG &= ~BIT1; // clear interrupt flag

}


