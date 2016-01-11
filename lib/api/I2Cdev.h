#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include "mbed.h"

#define I2CDEV_DEFAULT_READ_TIMEOUT 1000

class I2Cdev {
    public:
        I2Cdev();

        static int8_t readByte(uint8_t regAddr, uint8_t *data);
        static int8_t readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

        static bool writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);
        static bool writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
        static bool writeByte(uint8_t regAddr, uint8_t data);
        static bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
};

#endif // _I2CDEV_H_
