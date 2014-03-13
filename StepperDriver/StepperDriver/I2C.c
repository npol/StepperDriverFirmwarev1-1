/*
 * I2C.c
 *
 * Created: 1/10/2014 6:42:40 AM
 *  Author: Nishant Pol
 */ 

#include "I2C.h"

static uint8_t in_buf[4];                           /* Input Buffer for multi-byte commands */
static uint8_t in_buf_ptr = 0;                      /* Pointer to next empty byte */
static uint8_t cmd_size = 0;                        /* Total expected bytes for command */

static uint8_t out_buf[4];
static uint8_t out_buf_ptr = 0;
static uint8_t data_size = 0;

/* Initialize TWI Module */
void init_I2C(void){
    TWSR &= 0b11111100;		//clear prescaler bits
    TWBR = 0x00;			//No clocking since device is not master
    TWAR = (slave_addr << TWA0)|(1<<TWGCE);//assign slave address, enable general call
    TWCR = ((1<<TWEA)|(1<<TWEN))|(1<<TWIE);//enable recognition of address and enable hardware, enable interrupt
    return;
}

/** Main TWI Routine **/
ISR(TWI_vect)
{
    uint8_t TWI_Status = TWSR & 0xf8;
    switch(TWI_Status)
    {
        /* SR mode */
        case 0x60: /* Write mode transfer initiated, returned ACK */
        case 0x70: /* General call transfer initiated, returned ACK */
        in_buf_ptr = 0; /* Initialize a transmission */
        TWACK;
        break;
        
        case 0x80: /* Write mode transfer data received, returned ACK */
        case 0x90: /* General call transfer data received, returned ACK */
        in_buf[in_buf_ptr] = TWDR;
        if(in_buf_ptr == 0){
            cmd_size = get_cmd_size(in_buf[0]);
        }
        in_buf_ptr++;
        if(in_buf_ptr == cmd_size){
            proc_cmd(in_buf);
            in_buf_ptr = 0;    /* restore buffer for next request */
        }
        TWACK;
        break;
        
        case 0x88: /* Write mode transfer data received, returned NACK */
        case 0x98: /* General call transfer data received, returned NACK */
        TWNACK;
        break;
        
        /* ST mode */
        case 0xA8: /* Read mode transfer initiated, returned ACK */
        out_buf_ptr = 0;
        TWDR = out_buf[0];
        out_buf_ptr++;
        TWACK;
        break;
        
        case 0xB8: /* Read mode transfer data sent, ACK received */
        if (out_buf_ptr < data_size){
            TWDR = out_buf[out_buf_ptr];
            out_buf_ptr++;
            TWACK;
        } else {
            TWDR = 0x4f; /* Arbitrary code to indicate error */
            TWNACK;
        }
        break;
        
        case 0xC0: /* Read mode transfer data transmitted, NACK received */
        TWRESET;
        break;
        
        case 0xC8: /* Read mode transfer data transmitted, ACK received */
        TWRESET;
        break;
        
        /* Arbitration and Error cases */
        case 0x00: /* Illegal start or stop on bus */
        case 0x78: /* Arbitration lost in master mode, general call transfer initiated, returned ACK */
        case 0x68: /* Arbitration lost in master mode, new Write mode transfer initiated, returned ACK */
        case 0x10:
        case 0xA0: /* STOP or repeat START condition received while in SR mode */
        TWRESET;
        break;
    }
    return;
}
/** End Main TWI Routine **/
