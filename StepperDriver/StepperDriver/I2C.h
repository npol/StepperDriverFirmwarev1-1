/*
 * I2C.h
 *
 * Created: 1/10/2014 4:56:00 AM
 *  Author: Nishant Pol
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "cmd.h"

void init_I2C(void);