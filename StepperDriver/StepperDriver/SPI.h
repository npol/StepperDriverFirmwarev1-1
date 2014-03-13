/*
 * SPI.h
 *
 * Created: 1/10/2014 6:43:50 AM
 *  Author: Nishant Pol
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "cmd.h"

#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

void init_SPI(void);
uint8_t SPI_Receive(void);
void SPI_Transmit(uint8_t data);