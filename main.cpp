#include "mbed.h"
#include "I2Cdev.h"
#include "MPU6050.h"

// The offsets of the gyro
#define Gx_offset -1.50
#define Gy_offset 0
#define Gz_offset 0.80
// The offsets of the accel
#define Ax_offset -0.07
#define Ay_offset 0.02
#define Az_offset 0.14

const int* W=new int[71]{19112,-2698,89208,-7642,-7701,21354,-12261,6248,-11737,10025,6273,-7121,-1641,17265,-9006,-7766,-3560,-495,32684,1277,2183,3603,-18422,8568,-30245,-27630,16859,-33980,-39899,49234,-34294,
11711,-9884,12025,-14782,5552,-8769,1361,1192,1759,1500,-6706,3850,-14032,-21963,1135,-14026,-25131,-12057,15731,-15956,-4415,16678,-5724,9296,-2791,-13962,-5135,-4383,-19438,-22323,6830,
1878,16037,-61541,
-19352,-52467,36058,
-1547,30898,-9855,
};

MPU6050 accelgyro;

Serial serial(p9, p10);

int16_t ax, ay, az;

int* data;

/*
 * Watchdog control WDTOSCCTRL options
 * Divisor:
 * 2 x (1 + VAL) ie VAL = 0x0 -> Divisor = 2
 * Frequency:
 * 0x1 0.6    MHz
 * 0x2 1.05   MHz
 * 0x3 1.4    MHz
 * 0x4 1.75   MHz
 * 0x5 2.1    MHz
 * 0x6 2.4    MHz
 * 0x7 2.7    MHz
 * 0x8 3.0    MHz
 * 0x9 3.25   MHz
 * 0xA 3.5    MHz
 * 0xB 3.75   MHz
 * 0xC 4.0    MHz
 * 0xD 4.2    MHz
 * 0xE 4.4    MHz
 * 0xF 4.6    MHz
 */

int classify(int* v);
void sigmoid(int* var);

int main() {
    LPC_SYSCON->WDTOSCCTRL = (0x2 << 5) | 0x0;   // Sets the watchdog oscillator register | First hex is Frequency, Second is Divisor
    LPC_SYSCON->PDRUNCFG   &= ~(1 << 6);         // Powers on the watchdog oscillator
    LPC_SYSCON->MAINCLKSEL = 0x2;                // Sets watchdog oscillator as main clk
    LPC_SYSCON->MAINCLKUEN = 0x1;                //
    LPC_SYSCON->MAINCLKUEN = 0x0;                // Applies these changes
    LPC_SYSCON->MAINCLKUEN = 0x1;                //
    while (!(LPC_SYSCON->MAINCLKUEN & 0x01));    // Waits for changes to complete

    printf("Initializing I2C device.....\n");
    accelgyro.initialize();
    printf("Testing device connections....\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failure\n");

    data   = (int *)malloc(sizeof(int) * 32);
    int* i = data;

    while(1) {
        accelgyro.getAcceleration(&ax, &ay, &az);
        *i = ax >> 2;
        i++;
        *i = ay >> 2;
        i++;
        *i = az >> 2;
        i++;

        if (i == data + 30)
        {
            classify(data);
            free(data);
            data = (int *)malloc(sizeof(int) * 32);
            i    = data;
        }
    }
}

int classify(int* v)
{
    int* offset=(int*)W;
    int* s1;
    int* e1;
    int* s2=v;
    int* e2=s2 + 30;

    // Layer 2
    s1=v + 30;
    e1=s1 + 2;
    for (int* i = s1; i < e1; i++){
        *i=*offset++;
        for (int* j=s2; j < e2; j++) {
            *i+=(*offset++**j) >> 12;
        }
        sigmoid(i);
    }

    // Layer 1
    s2=v + 0;
    e2=s2 + 3;
    for (int* i = s2; i < e2; i++){
        *i=*offset++;
        for (int* j=s1; j < e1; j++) {
            *i+=(*offset++**j) >> 12;
        }
        sigmoid(i);
    }

    if (v[0] > v[1] && v[0] > v[2])
    {
        return 0;
    }
    else if (v[1] > v[0] && v[1] > v[2])
    {
        return 1 << 12;
    }
    else
    {
        return 2 << 12;
    }
}

void sigmoid(int* var)
{
    if (*var < -5 << 12)
    {
        *var = 0;
    }
    else if (*var > 5 << 12)
    {
        *var = 1 << 12;
    }
    else
    {
        *var = ((410 * *var) >> 12) + 2048;
    }
}
