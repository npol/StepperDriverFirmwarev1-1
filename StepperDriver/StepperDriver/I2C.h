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

/** Macro Helper Functions **/
/* Set TWI to send ACK on next transmission (normal case) */
#define TWACK (TWCR =	(1<<TWEN)|	/* Enable TWI Hardware */\
                        (1<<TWINT)|	/* Clear TWI Interrupt flag */\
                        (1<<TWIE)|	/* Enable TWI Interrupt */\
                        (1<<TWEA))	/* Send ACK on next transmission */

/* Set TWI to send NACK on next transmission (error case) */
#define TWNACK (TWCR =  (1<<TWEN)|	/* Enable TWI Hardware */\
                        (1<<TWINT)|	/* Clear TWI Interrupt flag */\
                        (1<<TWIE)|	/* Enable TWI Interrupt */\
                        (0<<TWEA))	/* Send NACK on next transmission */

/* Reset TWI for new transmission */
#define TWRESET (TWCR |=(1<<TWEN)|	/* Enable TWI Hardware */\
                        (1<<TWEA)|	/* Send ACK on start of transmission */\
                        (1<<TWIE))	/* Enable TWI Interrupt */
                        