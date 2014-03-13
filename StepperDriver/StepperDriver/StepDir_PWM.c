/*
 * StepDir.c
 *
 * Created: 1/10/2014 4:48:50 AM
 *  Author: Nishant Pol
 */ 

#include "StepDir_PWM.h"

inline static int8_t translate_pulse(uint16_t time);

static uint8_t prev_pinb;

void init_StepDir(void){
    PCICR |= (1<<PCIE0);    /* Enable PCINT3 for Step */
    PCMSK0 |= (1<<PCINT3);
    prev_pinb = PINB;
    return;
}

void init_pwm(void){
    TCCR1B = (1<<ICNC1)|    /* Enable Noise Canceling */
             (1<<ICES1)|     /* Capture on Falling Edge */
             (0b010<<CS00);  /* Run timer at f_osc/8 */
    TIMSK1 = (1<<ICIE1);    /* Enable Input Capture Interrupt */
    PCMSK0 |= 1<<PINB0;     /* Enable PCINT0 on PWM input to detect start of PWM cycle */
    PCICR |= (1<<PCIE0);
    prev_pinb = PINB;
    return;
}

ISR(PCINT0_vect){
    /* Step-Dir portion of interrupt vector */
    uint8_t pinb_temp = PINB;
    if((pinb_temp & (1<<STEP)) != (prev_pinb & (1<<STEP))){
        if(!(pinb_temp & (1<<EN)) && (pinb_temp & (1<<STEP)) && (pinb_temp & (1<<DIR))){
            desired_step_cnt++;
        } else if(!(pinb_temp & (1<<EN)) &&(pinb_temp & (1<<STEP)) && !(pinb_temp & (1<<DIR))){
            desired_step_cnt--;
        }
    }        
    
    /* PWM portion of interrupt vector */
    if((pinb_temp & (1<<PWM)) != (prev_pinb & (1<<PWM))){
        if(pinb_temp & (1<<PWM)){    /* High pulse started, reset timer */
            TCNT1 = 0;            
        } else {                /* High pulse done, read timer */
            desired_step_cnt += translate_pulse(TCNT1);
        }
    }
    prev_pinb = pinb_temp;
    return;
}

inline static int8_t translate_pulse(uint16_t time){
    return time & 0xff;
}

