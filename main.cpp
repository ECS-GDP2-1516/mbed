#include "MPU6050.h"
#include "classify.h"
#include "DigitalOut.h"

using namespace mbed;
using namespace std;

// The offsets of the gyro
#define Gx_offset -1.50
#define Gy_offset 0
#define Gz_offset 0.80
// The offsets of the accel
#define Ax_offset -0.07
#define Ay_offset 0.02
#define Az_offset 0.14

//Serial serial(p9, p10);
DigitalOut myled(LED2);
DigitalOut myled2(LED3);
DigitalOut myled3(LED4);
int16_t ax, ay, az;

int8_t rear = -1;
int buffer[BUFFER_SIZE]; //the buffer is just used to read values into

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
 
// inserts the next set of values read from the sensor into the buffer
void insert_reading(int b[], int x, int y, int z) {
    rear=(rear+3)%BUFFER_SIZE;
    b[rear-2]=x;
    b[rear-1]=y;
    b[rear]=z;
}

int main() {
    LPC_SYSCON->WDTOSCCTRL = (0x1 << 5) | 0x5;   // Sets the watchdog oscillator register | First hex is Frequency, Second is Divisor
    LPC_SYSCON->PDRUNCFG   &= ~(1 << 6);         // Powers on the watchdog oscillator
    LPC_SYSCON->MAINCLKSEL = 0x2;                // Sets watchdog oscillator as main clk
    LPC_SYSCON->MAINCLKUEN = 0x1;                //
    LPC_SYSCON->MAINCLKUEN = 0x0;                // Applies these changes
    LPC_SYSCON->MAINCLKUEN = 0x1;                //
    while (!(LPC_SYSCON->MAINCLKUEN & 0x01));    // Waits for changes to complete

    //LPC_USART->ACR = 0x7;
    init();

    //serial.printf("Testing device connections....\n");
    //serial.printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failure\n");
    myled3 = 1;

    //data   = (int *)malloc(sizeof(int) * CLASSIFY_MEMORY_ALLOCATION);
    //int* i = data;
    myled2 = 0;
    
    int count;
    
    //first, fill up the buffer with values
    for(count=0; count<BUFFER_SIZE/3; count++) {
        getAcceleration(&ax, &ay, &az);
        insert_reading(buffer, ax >> 2, ay >> 2, az >> 2);
    }
    myled2 = 1;
    //now we continue to read values and classify them
    while(1) {
        getAcceleration(&ax, &ay, &az);

        insert_reading(buffer, ax >> 2, ay >> 2, az >> 2);

        myled = classify(rear, buffer);
    }
}
