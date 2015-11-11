#include "I2Cdev.h"

I2Cdev::I2Cdev() {
}

I2Cdev::I2Cdev(PinName sda, PinName scl) {
}

int8_t I2Cdev::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    uint8_t ret = readByte(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return ret;
}

int8_t I2Cdev::readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint16_t b;
    uint8_t ret = readWord(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return ret;
}

/*
 * 76543210 bit numbers
 *    xxx   args: bitStart = 4, length = 3
 */
int8_t I2Cdev::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    uint8_t b;
    if(readByte(devAddr, regAddr, &b))
        return -1;
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return 0;
}

int8_t I2Cdev::readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data) {
    uint16_t b;
    if(readWord(devAddr, regAddr, &b))
        return -1;
    uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    return 0;
}

int8_t I2Cdev::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data) {
    return readBytes(devAddr, regAddr, 1, data);
}

int8_t I2Cdev::readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data) {
    return readWords(devAddr, regAddr, 1, data);
}

int8_t I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
    i2c.start();
    if(i2c.write(devAddr, &regAddr, 1)) {
        i2c.stop();
        return -1;
    }
    i2c.stop();
    return i2c.read(devAddr, data, length);
}

int8_t I2Cdev::readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
    i2c.start();
    if(i2c.write(devAddr, &regAddr, 1)) {
        i2c.stop();
        return -1;
    }
    for(uint8_t i = 0; i < length * 2; i++) {
        uint8_t b;
        if(readByte(devAddr, regAddr, &b)) {
            i2c.stop();
            return -1;
        }
        if((i % 2) == 0)
            data[i / 2] = b << 8;
        else
            data[i / 2] |= b;
    }
    i2c.stop();
    return 0;
}

bool I2Cdev::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(devAddr, regAddr, b);
}

bool I2Cdev::writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data) {
    uint16_t w;
    readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    return writeWord(devAddr, regAddr, w);
}

bool I2Cdev::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    if(readByte(devAddr, regAddr, &b))
        return true;
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1);
    data &= mask;
    b &= ~(mask);
    b |= data;
    return writeByte(devAddr, regAddr, b);
}

bool I2Cdev::writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data) {
    uint16_t w;
    if(readWord(devAddr, regAddr, &w))
        return true;
    uint16_t mask = ((1 << length) - 1) << (bistStart - length + 1);
    data <<= (bitStart - length + 1);
    data &= mask;
    w &= ~(mask);
    w |= data;
    return writeWord(devAddr, regAddr, w);
}

bool I2Cdev::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}

bool I2Cdev::writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
    return writeWords(devAddr, regAddr, 1, &data);
}

bool I2Cdev::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
    i2c.start();
    if(i2c.write(devAddr, &regAddr, 1)) {
        i2c.stop();
        return true;
    }
    if(i2c.write(devAddr, data, length)) {
        i2c.stop();
        return true;
    }
    i2c.stop();
    return false;
}


bool I2Cdev::writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
    uint8_t words[length * 2];
    for(uint8_t i = 0; i < length; i++) {
        words[length * 2] = (uint8_t)(data[length] >> 8);
        words[length * 2 + 1] = (uint8_t)(data[length] & 0xff);
    }
    i2c.start();
    i2c.write(devAddr, &regAddr, 1);
    if(i2c.write(devAddr, words, length * 2)) {
        i2c.stop();
        return true;
    }
    i2c.stop();
    return false;
}
