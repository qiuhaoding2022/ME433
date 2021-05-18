// initialize SPI1
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "spi.h"
#include <math.h>
#define pi 3.1415926
// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use internal oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // internal RC
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 36 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
void send_DAC(char AB, unsigned short val);


int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;
    TRISBbits.TRISB4 = 1;

    initSPI();
    
    __builtin_enable_interrupts();
    
    unsigned char i=0;
    float vA=0,vB=0,dt=0.001,tA=0,tB=0;
    int slope = 1024*2/(1/dt);
    while(1) {
        LATAbits.LATA0=1;
        vA=512+512*sin(4*pi*tA);
        vB=vB+slope;
        tB=tB+dt;
        tA=tA+dt;
        if(tB>=0.5){
            slope=slope*-1;
            tB=0;
        }
        send_DAC(0,(unsigned short)vA); //0 A, 1 B, max 2^10
        send_DAC(1,(unsigned short)vB); //0 A, 1 B, max 2^10   

        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() <48000000/2*dt) {}
    }
}

void send_DAC(char AB, unsigned short val){
    unsigned short p = AB << 15;
    p=p|(0b111<<12);
    p=p|(val<<2);
    //p=0b1111111111111100;
    LATAbits.LATA0=0; //bring CS low
    spi_io(p>>8);
    spi_io(p);        ;
    LATAbits.LATA0=1;
}


