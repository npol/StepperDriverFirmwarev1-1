/*
 * SPI.c
 *
 * Created: 1/10/2014 6:43:31 AM
 *  Author: Nishant Pol
 */ 

#include "SPI.h"
#include "cmd.h"

static uint8_t in_buf[4];                           /* Input Buffer for multi-byte commands */
static uint8_t in_buf_ptr = 0;                      /* Pointer to next empty byte */
static uint8_t cmd_size = 0;                        /* Total expected bytes for command */

void init_SPI(void){
    /* Note: SS held low by SEL2 jumper, so SPI is always active.  */
    DDRB |= (1<<MISO);  /* Set MISO as output, MOSI, SS, and SCK as inputs */
    SPCR = (1<<SPIE)|   /* Enable SPI and SPI interrupt */
           (1<<SPE)|
           (0<<DORD)|   /* MSB first */
           (0<<CPOL)|   /* SPI mode (0,0) */
           (0<<CPHA);
}

uint8_t SPI_Receive(void){
    while(!(SPSR & (1<<SPIF))); /* Wait for completion */
    return SPDR;
}
void SPI_Transmit(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));  /* Wait for completion */
    return;
}

ISR(SPI_STC_vect){
    while(!(SPSR & (1<<SPIF)));
    in_buf[in_buf_ptr] = SPDR;
    //SPI_Transmit(SPDR);
    if(in_buf_ptr == 0){                            /* First byte of command */
        cmd_size = get_cmd_size(in_buf[0]);
    }
    in_buf_ptr++;
    if(in_buf_ptr == cmd_size){   /* request ready, process command */
        proc_cmd(in_buf);
        in_buf_ptr = 0;    /* restore buffer for next request */
    }
    return;
}