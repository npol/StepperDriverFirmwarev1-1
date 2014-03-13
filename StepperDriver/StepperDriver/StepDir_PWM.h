/*
 * StepDir.h
 *
 * Created: 1/10/2014 4:48:36 AM
 *  Author: Nishant Pol
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include "StepperDriver.h"
#include "cmd.h"

#define PWM PINB0
#define STEP PINB3
#define DIR PINB4
#define EN PINB5

void init_StepDir(void);
void init_pwm(void);