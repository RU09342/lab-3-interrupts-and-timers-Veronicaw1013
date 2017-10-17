#include <msp430.h>

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;//disable high impedance mode
    P1DIR |= BIT0 + BIT1;   // pins 1.0 and 1.1 are set as outputs
    P5REN|= BIT6;   // pullup or pulldown resistor enabled
    P5OUT|= BIT6;   // pullup resistor selected
    P5IE |= BIT6;   // interrupt enable on port 5.6
    P5IES |= BIT6;  // set interrupt to falling edge
    P5IFG &= ~BIT6; // clear interrupt flag
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


#pragma vector=PORT5_VECTOR     // button interrupt

    __interrupt void PORT_5(void)

    {

        if (P5IES & BIT6) // if button is pressed...

        {

            TA1CTL = TASSEL_1 + MC_2;   // timerA1 is set to ACLK, continuous mode

            P1OUT ^= BIT1;              // toggle second LED

            P5IES &= ~BIT6;             // set interrupt to rising edge

        }

        else                // if button is depressed...

        {

            TA0CCR0 = TA1R; // set CCR0 to the value counted in TA1R

            P1OUT &= ~BIT1; // turn off the second LED

            P5IES |= BIT6; // set interrupt to falling edge

            TA1CTL = TACLR; // clear timerA1

        }



        P5IFG &= ~BIT6; // clear interrupt flag

}

