#include "mbed.h"

DigitalOut myled(LED1);
Serial serial(p9, p10);
I2C sensor(p28, p27);
uint8_t address = 210;

/*int main() {
    while(1) {
        myled = 1;
        serial.printf("Hello world\n");
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}*/

int main() {
    while(1) {
        char data[1];
        int ret;
        for(address = 0; address < 256; address++) {
            ret = sensor.read(address, data, 1);
            if(ret == 0) 
                printf("%d, %d\n", address, ret);
        }
    }
}
