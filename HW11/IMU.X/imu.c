#include "imu.h"
#include <xc.h>
#include "i2c_master_noint.h"
void imu_setup(){
    int whoami= i2c_read(WHO_AM_I);
    if (whoami != 105){
        while(1);
    }
    i2c_write(CTRL1_XL,0b10000010); //1.66kHz sample rate 2g sensitivity 100Hz filter
    i2c_write(CTRL2_G,0b10001000); // 1.66kHz 1000 dps sensitivity
    i2c_write(CTRL3_C,0b00000100); // enable IF_INC}
}