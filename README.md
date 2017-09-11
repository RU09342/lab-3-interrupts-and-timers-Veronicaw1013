# Lab 3: Timers and Interrupts
During Lab 2, you probably noticed a few unexpected behaviors when trying to program a simple LED blink code. For starters, did you notice that it wasn't as straight forward to have the LED blink at a specific interval (it was not just a simple Clock/Blinking Rate to figure out the required delays). And when you had to deal with multiple LED's blinking at different rates, could you easily maintain the timing between the edges? What about if you wanted the LEDs to be on/off at rates (otherwise known as duty cycle)?

And what about that button? Do we really need to keep checking whether or not the button was pressed at the fastest rate possible? What about if we wanted to perform an action on just a falling or rising edge? 