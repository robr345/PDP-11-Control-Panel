/*
 * File:   powplus.c
 * Author: robr345
 *
 * Created on 13 September 2024, 10:31
 * 
 * for PIC16F628A
 * 
 * PDP-11 switch panel source code.
 * allow the processor to start
 * then look for a reset 
 * or halt the processor to run ODT
 */

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#include <stdbool.h>

#define _XTAL_FREQ 32768

#define BDCOK  RA0
#define BPOK   RA1
#define BHALT  RA2

#define SYS_RESET_BTN  RB0      // SW1
#define CPU_HALT_BTN   RB1      // SW2
#define AUX_BTN        RA3      // SW3

#define SRUN   RB2
#define BEVNT  RB3

#define RUN_LED   RB7
#define HALT_LED  RB6
#define AUX_LED   RB5

void main()
{   
    bool haltButtonDown = false;
    bool resetButtonDown = false;
    bool auxButtonDown = false;
    
    CMCON = 0x07;   // disable comparator
    VRCON = 0x00;
    
    // 0 == output; 1 == input
    TRISA0 = 0;     // BDCOK
    TRISA1 = 0;     // BPOK
    TRISA2 = 0;     // BHALT; high to allow the processor to run
    TRISB2 = 1;     // SRUN; low when processor is running
    TRISB3 = 0;     // BEVNT/LTC is outputting signal by default
    
    //TRISA3 = 1;     // AUX button
    //TRISB0 = 1;     // SYS_RESET_BTN
    //TRISB1 = 1;     // CPU_HALT_BTN
    
    TRISB5 = 0;     // aux led output
    TRISB6 = 0;     // halt led output
    TRISB7 = 0;     // run led output
    
    // set leds
    
    RUN_LED  = 0;   // RUNNING
    HALT_LED = 0;   // HALTED
    
    
    // set PDP-11 lines low
    BEVNT = 0;
    BDCOK = 0;
    BPOK  = 0;
    
    // allow the processor to run
    BHALT = 1;
    
    // 32.768khz crystal
    // output 60Hz signal on pin 9 (RB3)
    T2CON   = 0b00000100;
    PR2     = 0b10001000;
    CCPR1L  = 0b01000100;
    CCP1CON = 0b00011100;
    
    // set LTC clock as on by default
    bool t2Running = true;
    AUX_LED = 1;   // turn on AUX led
    
    
    // let things settle
    __delay_ms(1000);
    __delay_ms(500);
    // signal processor to start
    BDCOK = 1;
    __delay_ms(10);
    BPOK = 1;
    // delay while processor performs start up
    __delay_ms(100);
    
    
    RUN_LED = 1;    // turn on RUN led
    __delay_ms(500); 
    
    
    // main program loop
    while(1)
    {
        if (resetButtonDown == false) {
            if (SYS_RESET_BTN == 0) {   // reset button down
                resetButtonDown = true;
                
                t2Running = true;     // make sure LTC is running
                TRISB3 = 0;           // BEVNT/LTC switch to output
                AUX_LED  = 1;         // turn led on
                
                BHALT = 1;              // make sure processor is running
                HALT_LED = 0;           // make sure led of off
                haltButtonDown = false; // make sure this is false
                __delay_ms(100);        // give is some time
                
                RUN_LED = 0;            // led off
                BPOK = 0;               // start reset
                __delay_ms(100);
                BDCOK = 0;
                __delay_ms(100);
            
                BDCOK = 1;              // end reset
                __delay_ms(10);
                BPOK = 1;
                __delay_ms(100);
                RUN_LED = 1;            // led on
            
            }
        }
        
        if (haltButtonDown == false) {
            if (CPU_HALT_BTN == 0) {        // halt button down
                haltButtonDown = true;
                if (BHALT == 1) {           // processor running
                    BHALT = 0;              // halt the processor
                    HALT_LED = 1;           // turn on led
                } else {                    // processor not running
                    BHALT = 1;              // start the processor
                    HALT_LED = 0;           // turn off led
                }
            }
        }
        
        // aux button is programmed to control the LTC clock
        // LTC clock is on by default
        if (auxButtonDown == false) {
            if (AUX_BTN == 0) {         // aux button down
                auxButtonDown = true;
                if (t2Running) {        // if timer is running turn it off
                    t2Running = false;
                    TRISB3 = 1;         // BEVNT/LTC switch to input
                    AUX_LED  = 0;       // turn led off
                } else {
                    t2Running = true;
                    TRISB3 = 0;         // BEVNT/LTC switch to output
                    AUX_LED  = 1;       // turn led on
                }
            }
        }
        
        // reset button de-bounce
        // button needs to test being up 3 times before
        // resetButtonDown switch is set to false
        if (resetButtonDown) {
            if (SYS_RESET_BTN == 1) {                   // if reset button up
                __delay_ms(5);
                if (SYS_RESET_BTN == 1) {               // if reset button still up
                    __delay_ms(5);
                    if (SYS_RESET_BTN == 1) {           // if reset button still up
                        resetButtonDown = false;        // button up no bounce
                    }
                }
            }
        }
        
        
        // halt button de-bounce
        // button needs to test being up 3 times before
        // haltButtonDown switch is set to false
        if (haltButtonDown) {
            if (CPU_HALT_BTN == 1) {                // if halt button up
               __delay_ms(5);
               if (CPU_HALT_BTN == 1) {             // if halt button still up
                    __delay_ms(5);
                    if (CPU_HALT_BTN == 1) {        // if halt button still up
                        haltButtonDown = false;     // button up, no bounce
                    }
               }
            }         
        }
        
        // aux button de-bounce
        // button needs to test being up 3 times before
        // auxButtonDown switch is set to false
        if (auxButtonDown) {
            if (AUX_BTN == 1) {                     // if halt button up
               __delay_ms(5);
               if (AUX_BTN == 1) {                  // if halt button still up
                    __delay_ms(5);
                    if (AUX_BTN == 1) {             // if halt button still up
                        auxButtonDown = false;     // button up, no bounce
                    }
               }
            }         
        }
        
		// small delay before looping again
        __delay_ms(10); 
      
    }   // end while
}
