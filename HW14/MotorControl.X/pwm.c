#include <xc.h>   
#include<sys/attribs.h>  // __ISR macro
#include "pwm.h"
void pwm_init()
{
RPB6Rbits.RPB6R=0b0110;
T2CONbits.TCKPS=0b111; //1:256 
PR2 = 3749;
TMR2=0;
OC5CONbits.OCM = 0b110; 
OC5RS = 140; // duty cycle = OC1RS/(PR2+1) = 25%
OC5R = 140; // initialize before turning OC1 on; afterward it is read-only
T2CONbits.ON = 1; // turn on Timer2
OC5CONbits.ON = 1; // turn on OC1
_CP0_SET_COUNT(0);
}
    
void pwm_set(int a){
    if(a>OC5RS_MAX)
    {
        OC5RS= OC5RS_MAX;
    }
    else if(a<OC5RS_MIN){
        OC5RS= OC5RS_MIN;
    }
    else  {OC5RS= a;}
}