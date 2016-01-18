// I2Cdev library collection - MPU6050 I2C device class
// Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
// 10/3/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     ... - ongoing debug release

// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "i2c_api.h"

/**
 * Addresses in the I2C
 */
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C


static int8_t readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
    int written = i2c_write((const char *)&regAddr, 1);
    if(1 != written)
        return -1;

    int read = i2c_read((char *)data, length);

    return length != read;
}

static inline void writeBytes(uint8_t regAddr, uint8_t *data) {
    uint8_t send[2];
    send[0] = regAddr;
    send[1] = data[0];
    i2c_write((const char *)send, 2);
}

static void writeBits(uint8_t regAddr, uint8_t mask, uint8_t data) {
    uint8_t b;
    if(readBytes(regAddr, 1, &b))
        return;

    b &= mask;
    b |= data;
    
    writeBytes(regAddr, &b);
}

static inline void init()
{
    i2c_init();

    writeBits(MPU6050_RA_PWR_MGMT_1, 0x90, 0x28);
    writeBits(MPU6050_RA_GYRO_CONFIG, 0xE7, 0x00);
    writeBits(MPU6050_RA_ACCEL_CONFIG, 0xE7, 0x00);
    writeBits(MPU6050_RA_PWR_MGMT_2, (uint8_t)~0xC7, 0x87);
}

static inline void getAcceleration(int16_t* buffer) {
    uint8_t buf[6];

    readBytes(MPU6050_RA_ACCEL_XOUT_H, 6, (uint8_t*)buf);

    buffer[0] = (((uint16_t)buf[0]) << 8) | (buf[1]);
    buffer[1] = (((uint16_t)buf[2]) << 8) | (buf[3]);
    buffer[2] = (((uint16_t)buf[4]) << 8) | (buf[5]);

    buffer[0] >>= 2;
    buffer[1] >>= 2;
    buffer[2] >>= 2;
}

#endif /* _MPU6050_H_ */
