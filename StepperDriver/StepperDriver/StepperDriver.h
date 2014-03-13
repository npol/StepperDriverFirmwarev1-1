/*
 * StepperDriver.h
 *
 * Created: 1/3/2014 9:16:21 PM
 *  Author: Nishant Pol
 */ 
#define F_CPU 1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "StepDir_PWM.h"
#include "UART.h"
#include "SPI.h"
#include "I2C.h"

/** Pinout **/

#define CHOPCD PINC0
#define CHOPAB PINC1

#define SPD PINC3

#define A PIND2
#define B PIND3
#define C PIND4
#define D PIND7
#define ABINH PIND5
#define CDINH PIND6

#define SEL1 PINB1
#define SEL2 PINB2


 