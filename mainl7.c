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

// Constantes
#define _tmr0_value 236

// Variables
uint8_t banderas;
uint8_t centenas;
uint8_t decenas;
uint8_t unidades;
uint8_t res;
uint8_t tabla[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
uint8_t i;

// Funciones
uint8_t udc(uint8_t a);         // Prototipo de función

uint8_t udc(uint8_t a) {
    centenas = a/100;
    res = a%100;
    decenas = res/10;
    unidades = res%10;
}


void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    OSCCONbits.IRCF = 0b0101;   // 2MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    OPTION_REGbits.T0CS = 0;    // TMR0 con internal clock
    OPTION_REGbits.PSA = 0;     // Prescaler a TMR0
    OPTION_REGbits.PS = 0b0111; // PSA 256
    
    
    
    TRISA = 0;                  // PORTA como salida
    PORTA = 0;                  // Se limpia PORTA
    
    TRISB = 0b00000011;         // PORTB0 y PORTB1 como entradas
    PORTB = 0;                  // Se limpia PORTB
    
    TRISC = 0;                  // PORTC como salida
    PORTC = 0;                  // Se limpia PORTC
    
    TRISD = 0b11111000;         // PORTD como salida
    PORTD = 0;                  // Se limpia PORTD
    
    OPTION_REGbits.nRBPU = 0;   // Se habilitan los pullups
    WPUBbits.WPUB0 = 1;         // Se habilita el pullup en PORTB0
    WPUBbits.WPUB1 = 1;         // Se habilita el pullup en PORTB1
    
    INTCONbits.GIE = 1;         // Se habilitan las interrupciones globales
    INTCONbits.RBIE = 1;        // Se habilita interrupción del PORTB
    IOCBbits.IOCB0 = 1;         // Se habilita interrupción en PORTB0
    IOCBbits.IOCB1 = 1;         // Se habilita interrupción en PORTB1
    INTCONbits.RBIF = 0;        // Se limpia bandera de interrupción del PORTB
    INTCONbits.T0IE = 0;        // Se habilita interrupción en TMR0
    INTCONbits.T0IF = 0;        // Se limpia bandera de interrupción del TMR0
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
    
    if(INTCONbits.T0IF)
    {
        PORTD = 0;
        if (banderas == 0b00){
            PORTC = tabla[centenas];
            RD0 = 1;
            banderas = 0b01;
        }
        else if (banderas == 0b01){
            PORTC = tabla[decenas];
            RD1 = 1;
            banderas = 0b10;
        }
        else if (banderas == 0b10){
            PORTC = tabla[unidades];
            RD2 = 1;
            banderas = 0b00;
        }
       INTCONbits.T0IF = 0;
       TMR0 = _tmr0_value;
    }
    return;
    
}

void main(void) {
    setup();
    while(1){
       udc(PORTA);
    }
    return ;
}

        
        
        