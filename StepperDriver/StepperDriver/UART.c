/*
 * UART.c
 *
 * Created: 1/10/2014 4:53:35 AM
 *  Author: Nishant Pol
 */ 

#include "UART.h"

static uint8_t in_buf[4];                           /* Input Buffer for multi-byte commands */
static int8_t in_buf_ptr = 0;                      /* Pointer to next empty byte */
static uint8_t cmd_size = 0;                        /* Total expected bytes for command */

void init_UART(uint16_t baud_prescaler, uint8_t multiplier){
    UBRR0H = (uint8_t)(baud_prescaler >> 8);
    UBRR0L = (uint8_t)baud_prescaler;
    UCSR0A |= (multiplier << U2X0);                 /* Double Transmission Speed */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);     /* Enable RX, TX, and RX complete interrupt */
    UCSR0C = (0<<UMSEL00)|                          /* Asynchronous UART */
            (0<<UPM00)|                              /* No Parity */
            (0<<USBS0)|                              /* 1 Stop Bit */
            (3<<UCSZ00);                             /* 8 bit data */
    return;
}

/* Transmit 1-byte data */
void UART_Transmit(uint8_t data){
    while(!(UCSR0A & (1<<UDRE0)));                  /* Wait for empty buffer */
    UDR0 = data;
    return;
}

/* Receive 1 byte data */
uint8_t UART_Receive(void){
    uint8_t data;
    while(!(UCSR0A & (1<<RXC0)));                   /* Wait for RX complete */
    data = UDR0;
    return data;
}

/* RX complete interrupt */
ISR(USART_RX_vect){
    in_buf[in_buf_ptr] = UDR0;
    UART_Transmit(UDR0);
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
