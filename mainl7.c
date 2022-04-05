/* 
 * File:   mainl7.c
 * Author: saras
 *
 * Created on April 4, 2022, 9:11 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    TRISA = 0;                  // PORTA como salida
    PORTA = 0;                  // Se limpia PORTA
    
    TRISB = 0b00000011;         // PORTB0 y PORTB1 como entradas
    PORTB = 0;                  // Se limpia PORTB
    
    OPTION_REGbits.nRBPU = 0;   // Se habilitan los pullups
    WPUBbits.WPUB0 = 1;         // Se habilita el pullup en PORTB0
    WPUBbits.WPUB1 = 1;         // Se habilita el pullup en PORTB1
    
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.RBIE = 1;        // Se habilita interrupci�n del PORTB
    IOCBbits.IOCB0 = 1;         // Se habilita interrupci�n en PORTB0
    IOCBbits.IOCB1 = 1;         // Se habilita interrupci�n en PORTB1
    INTCONbits.RBIF = 0;        // Se limpia bandera de interrupci�n del PORTB
}

void __interrupt() isr (void)
{
    if(INTCONbits.RBIF)
    {
        if (!RB0){
            PORTA++;
        INTCONbits.RBIF = 0; }
        else if (!RB1){
            PORTA--;
        INTCONbits.RBIF = 0; }
    }
    return;
    
}

void main(void) {
    setup();
    while(1){
        
    }
    return ;
}