#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "i2c_master_noint.h"
#include "spi.h"
#include "ST7789.h"
#include "imu.h"
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

//unsigned char Wadd=0b01000000;
//unsigned char Radd=0b01000001;

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
    LCD_init();
    i2c_master_setup();
    imu_setup();
    __builtin_enable_interrupts();
    LCD_clearScreen(BLACK);
    signed short temp[1],acc[3];
    signed char zacc;
    char c[100];
    signed int x,y;
    while (1) {
        LATAbits.LATA4 = 0;
        delay();
        LATAbits.LATA4 = 1;
        delay();
        I2C_read_multiple(0x20,temp,1);
        I2C_read_multiple(0x28,acc,3);
    
        if (PORTBbits.RB4 == 0){
            LCD_clearScreen(BLACK);
            sprintf(c,"Temperature is %.2f degree C.            ",(float)((temp[0]+11800)/524.28));
            LCD_drawStr(28,40,c,RED);
            sprintf(c,"X acc reading is %d .          ",acc[0]);
            LCD_drawStr(28,48,c,RED);
            sprintf(c,"Y acc reading is %d .          ",acc[1]);
            LCD_drawStr(28,56,c,RED);
            sprintf(c,"Z acc reading is %d .          ",acc[2]);
            LCD_drawStr(28,64,c,RED);
        }
        else
        {  
            LCD_clearScreen(BLACK);
            x=acc[1]*130/16300;
            y=acc[0]*130/16300;
            LCD_drawX(x,CYAN);
            LCD_drawY(y,CYAN);
        }
    }
}
void delay(void) {
    _CP0_SET_COUNT(0);
    int a=_CP0_GET_COUNT();
    while(a<=48000000/1000) {
        a=_CP0_GET_COUNT();  
    }
  }
