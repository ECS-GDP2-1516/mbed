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

MPU6050 accelgyro;

DigitalOut myled(LED1);
Serial serial(p9, p10);

int16_t ax, ay, az;
int16_t gx, gy, gz;

float Ax, Ay, Az;
float Gx, Gy, Gz;

/*int main() {
    while(1) {
        char data[1];
        int ret;
        for(address = 0; address < 256; address++) {
            ret = sensor.read(address, data, 1);
            if(ret == 0) 
                printf("%d, %d\n", address, ret);
        }
    }
}*/

int main() {
    printf("Initializing I2C device.....\n");
    accelgyro.initialize();
    printf("Testing device connections....\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failure\n");
    while(1) {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        Ax = ax / 16384.00;
        Ay = ay / 16384.00;
        Az = az / 16384.00;
        Gx = gx / 131.00;
        Gy = gy / 131.00;
        Gz = gz / 131.00;
        printf("%d, %d, %d\n", Ax, Ay, Az);
        printf("%d, %d, %d\n", Gx, Gy, Gz);
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}
