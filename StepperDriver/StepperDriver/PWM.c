/*
 * PWM.c
 *
 * Created: 1/10/2014 4:48:00 AM
 *  Author: Nishant Pol
 */ 

#include "PWM.h"

void init_pwm(void){
    TCCR1B = (1<<ICNC1)|    /* Enable Noise Canceling */
            (1<<ICES1)|     /* Capture on Falling Edge */
            (0b010<<CS00);  /* Run timer at f_osc/8 */
    TIMSK1 = (1<<ICIE1);    /* Enable Input Capture Interrupt */
    PCMSK0 |= 1<<PINB0;     /* Enable PCINT0 on PWM input to detect start of PWM cycle */
    PCICR |= (1<<PCIE0);
}

ISR(PCINT0_vect){
    if(((PINB >> PINB0) & 1) == 1){ /* Clear timer 1 on rising edge */
        TCNT1 = 0;
    }
    return;
}

ISR(InputChangeVector){             /* Read duty cycle on falling edge */
    uint16_t duty = ICR1;
    /* Translate ICR1 into N-step */
    return;
}

