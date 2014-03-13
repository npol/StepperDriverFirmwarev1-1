/*
 * cmd.c
 *
 * Created: 1/10/2014 7:02:47 AM
 *  Author: Nishant Pol
 */ 

#include "cmd.h"


uint8_t motor_enable = OFF;

uint8_t slave_addr = 0x10;
int8_t desired_step_cnt = 0;

/* Returns total number of bytes expected from master */
uint8_t get_cmd_size(uint8_t cmd){
    uint8_t size = 0;
    switch(cmd){
        case 0x10:
        case 0x11:
        case 0x20:
        case 0x21:
            size = 1;
            break;
        case 0x22:
        case 0x23:
        case 0x30:
        case 0x80:
            size = 2;
            break;
    }
    return size;
}

/* Executes command, recieve_data is entry in input buffer */
void proc_cmd(uint8_t *recieve_data){
    switch(recieve_data[0]){
        case 0x10:
            motor_enable = 1;
            break;
            
        case 0x11:
            motor_enable = 0;
            break;
            
        case 0x20:
            desired_step_cnt++;
            break;
            
        case 0x21:
            desired_step_cnt--;
            break;
            
        case 0x22:
            desired_step_cnt += recieve_data[1];
            break;
            
        case 0x23:
            desired_step_cnt -= recieve_data[1];
            break;
            
        case 0x30:
            break;
            
        case 0x80:
            slave_addr = recieve_data[1];
            break;
            
    }
    return;
}