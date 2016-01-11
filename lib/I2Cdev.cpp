#include "I2Cdev.h"

I2C i2c(p28, p27);

I2Cdev::I2Cdev() {
}

int8_t I2Cdev::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    if(i2c.write((const char *)&regAddr, 1))
        return -1;
    int ret = i2c.read((char *)data, length);
    return ret;
}

void I2Cdev::writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    readBytes(regAddr, 1, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    writeBytes(regAddr, 1, &b);
}

void I2Cdev::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    if(readBytes(regAddr, 1, &b))
        return;
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1);
    data &= mask;
    b &= ~(mask);
    b |= data;
    
    writeBytes(regAddr, 1, &b);
}

void I2Cdev::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    uint8_t send[length + 1];
    send[0] = regAddr;
    for(int i = 0; i < length; i++)
        send[i + 1] = data[i];
    i2c.write((const char *)send, length + 1);
}
