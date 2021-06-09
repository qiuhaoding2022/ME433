#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "i2c_master_noint.h"
#include "spi.h"
#include "ST7789.h"

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

void delay(void);
void i2cwrite(char reg,char val);
unsigned char i2cread(char reg);
//unsigned char Wadd=0b01000000;
//unsigned char Radd=0b01000001;
#define Radd 0b11010101
#define Wadd 0b11010100
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
    __builtin_enable_interrupts();
    
    i2c_master_setup();
    
    while (1) {
        LATAbits.LATA4 = 0;
        LATAbits.LATA4 = 1;

       (i2cread(0xf)==0)
        {
            while(i2cread(0x13)==0){
            i2cwrite(0x14,0xFF);
            }
            i2cwrite(0x14,0x00);
        }

        
          
    }
}
void delay(void) {
    _CP0_SET_COUNT(0);
    int a=_CP0_GET_COUNT();
    while(a<=48000000/4) {
        a=_CP0_GET_COUNT();  
    }
  }

void i2cwrite(char reg,char val){
    i2c_master_start();
    i2c_master_send(Wadd); // send a byte (either an address or data)
    i2c_master_send(reg);
    i2c_master_send(val);
    i2c_master_stop();
}
unsigned char i2cread(char reg){
    i2c_master_start();
    i2c_master_send(Wadd); // send a byte (either an address or data)
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(Radd);
    int val=i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return val;
}
