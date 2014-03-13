/*
 * UART.h
 *
 * Created: 1/10/2014 4:53:51 AM
 *  Author: Nishant Pol
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "cmd.h"

void init_UART(uint16_t baud_prescaler, uint8_t multiplier);
uint8_t UART_Receive(void);
void UART_Transmit(uint8_t data);