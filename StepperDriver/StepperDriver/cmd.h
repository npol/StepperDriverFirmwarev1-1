/*
 * cmd.h
 *
 * Created: 1/10/2014 7:03:07 AM
 *  Author: Nishant Pol
 */ 


#include <avr/io.h>

uint8_t get_cmd_size(uint8_t cmd);
void proc_cmd(uint8_t *recieve_data);

#define OFF 0
#define FULL 1
#define HALF 2
#define WAVE 3
uint8_t motor_enable;

uint8_t slave_addr;
int8_t desired_step_cnt;
