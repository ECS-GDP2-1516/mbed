#include "I2Cdev.h"

I2Cdev::I2Cdev(PinName sda, PinName scl) {
}

int8_t I2Cdev::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout) {
    uint8_t b;
    uint8_t ret = readByte(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return ret;
}

int8_t I2Cdev::readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    uint16_t b;
    uint8_t ret = readWord(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return ret;
}

/*
 * 76543210 bit numbers
 *    xxx   args: bitStart = 4, length = 3
 */
int8_t I2Cdev::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    uint8_t b;
    if(readByte(devAddr, regAddr, &b))
        return -1;
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return 0;
}

int8_t I2Cdev::readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    uint16_t b;
    if(readWord(devAddr, regAddr, &b))
        return -1;
    uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return 0;
}

int8_t I2Cdev::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    return readByte(devAddr, regAddr, 1, data);

int8_t I2Cdev::readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    return readWords(devAddr, regAddr, 1, data);
}

int8_t I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
    if(i2c.write(devAddr, regAddr, 1))
        return -1;
    return i2c.read(devAddr, data, length);
}

int8_t I2Cdev::readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout) {
    if(i2c.write(devAddr, regAddr, 1))
        return -1;

}

bool I2Cdev::writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);

bool I2Cdev::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);

bool I2Cdev::writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);

bool I2Cdev::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);

bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);

bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);

bool I2Cdev::writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

uint16_t I2Cdev::readTimeout;
