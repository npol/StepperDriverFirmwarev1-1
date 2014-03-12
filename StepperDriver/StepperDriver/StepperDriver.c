/*
 * StepperDriver.c
 *
 * Created: 1/3/2014 9:15:45 PM
 *  Author: Nishant Pol
 */ 

#include "StepperDriver.h"

/** Function Prototypes **/

/* Helper functions */
inline static void init_interface(void);
inline static void init_stepper(void);
inline static void init_chopper(void);

/* Half step mode */
inline static void halfstep1(void);
inline static void halfstep2(void);
inline static void halfstep3(void);
inline static void halfstep4(void);
inline static void halfstep5(void);
inline static void halfstep6(void);
inline static void halfstep7(void);
inline static void halfstep8(void);

/* Normal mode */
inline static void normstep1(void);
inline static void normstep2(void);
inline static void normstep3(void);
inline static void normstep4(void);

/* Wave mode */
inline static void wavestep1(void);
inline static void wavestep2(void);
inline static void wavestep3(void);
inline static void wavestep4(void);

/** Function Definitions **/

int main(void)
{
    init_interface();
    init_stepper();
    init_chopper();
    sei();
    while(1)
    {
        normstep1();
        _delay_ms(100);
        normstep2();
        _delay_ms(100);
        normstep3();
        _delay_ms(100);
        normstep4(); 
        _delay_ms(100);
    }
    return 0;
}

inline static void init_interface(void){
    /* SEL1 and SEL2 input */
    uint8_t Select = (PINB >> SEL1) & 3;
    if(Select == 0b00){         /* PWM */
        init_pwm();
    } else if(Select == 0b01){  /* UART */
        //init_UART();
    } else if(Select == 0b10){  /* I2C */
        //init_I2C();
    } else if(Select == 0b11){  /* SPI */
        //init_SPI();
    }
    return;
}

/** Stepper Controller Functions **/
/* Half step mode */
inline static void init_stepper(void){
    DDRD = (1<<A)|(1<<B)|(1<<C)|(1<<D)|(1<<ABINH)|(1<<CDINH);
    //halfstep1();
}

inline static void halfstep1(void){
    PORTD = (0<<A)|(1<<B)|(0<<C)|(1<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void halfstep2(void){
    PORTD = (0<<A)|(0<<B)|(0<<C)|(1<<D)|(0<<ABINH)|(1<<CDINH);
}

inline static void halfstep3(void){
    PORTD = (1<<A)|(0<<B)|(0<<C)|(1<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void halfstep4(void){
    PORTD = (1<<A)|(0<<B)|(0<<C)|(0<<D)|(1<<ABINH)|(0<<CDINH);
}

inline static void halfstep5(void){
    PORTD = (1<<A)|(0<<B)|(1<<C)|(0<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void halfstep6(void){
    PORTD = (0<<A)|(0<<B)|(1<<C)|(0<<D)|(0<<ABINH)|(1<<CDINH);
}

inline static void halfstep7(void){
    PORTD = (0<<A)|(1<<B)|(1<<C)|(0<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void halfstep8(void){
    PORTD = (0<<A)|(1<<B)|(0<<C)|(0<<D)|(1<<ABINH)|(0<<CDINH);
}

/* Normal mode */
inline static void normstep1(void){
    PORTD = (0<<A)|(1<<B)|(0<<C)|(1<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void normstep2(void){
    PORTD = (1<<A)|(0<<B)|(0<<C)|(1<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void normstep3(void){
    PORTD = (1<<A)|(0<<B)|(1<<C)|(0<<D)|(1<<ABINH)|(1<<CDINH);
}

inline static void normstep4(void){
    PORTD = (0<<A)|(1<<B)|(1<<C)|(0<<D)|(1<<ABINH)|(0<<CDINH);
}

/* Wave step or 1 phase mode */
inline static void wavestep1(void){
    PORTD = (0<<A)|(0<<B)|(0<<C)|(1<<D)|(0<<ABINH)|(1<<CDINH);
}

inline static void wavestep2(void){
    PORTD = (1<<A)|(0<<B)|(0<<C)|(0<<D)|(1<<ABINH)|(0<<CDINH);
}

inline static void wavestep3(void){
    PORTD = (0<<A)|(0<<B)|(1<<C)|(0<<D)|(0<<ABINH)|(1<<CDINH);
}

inline static void wavestep4(void){
    PORTD = (0<<A)|(1<<B)|(0<<C)|(0<<D)|(1<<ABINH)|(0<<CDINH);
}

/* Chopper Current Control on Inhibit lines */
inline static void chopABINH(void){
    PORTD &= ~(1<<ABINH);
}

inline static void chopCDINH(void){
    PORTD &= ~(1<<CDINH);
}

/* Chopper Current Control on signal lines */
inline static void init_chopper(void){
    PCMSK1 |= (1<<PCINT8)|                           /* PC0: CHOPCD */
            (1<<PCINT9);                            /* PC1: CHOPAB */
    PCICR |= 1<<PCIE1;                               /* Enable PCI1 Interrupt for PC0 (PCINT8) and PC1 (PCINT9) */
}

/** END Stepper Controller Functions **/

/* Chopper Interrupt Routine */
ISR(PCINT1_vect){
    uint8_t Comp_Input = PINC & 3;
    uint8_t chop_mode = 0;                          /* 0 if chop on inhibit, 1 if chop on lines */
    if((Comp_Input == 0b01) || (Comp_Input == 0b11)){       /* CHOPCD */
        if(chop_mode == 0){
            PORTD &= ~(1<<CDINH);
        } else{
            PORTD &= ~((1<<C)|(1<<D));
        }
    } else if((Comp_Input == 0b10) || (Comp_Input == 0b11)){/* CHOPAB */
        if(chop_mode == 0){
            PORTD &= ~(1<<ABINH);
        } else {
            PORTD &= ~((1<<A)|(1<<B));
        }
    }
    return;
}

/* End Chopper Interrupt Routine */
