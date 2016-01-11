/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_I2C_API_H
#define MBED_I2C_API_H

#include "device.h"
#include "PinNames.h"

/**
 * @defgroup I2CEvents I2C Events Macros
 *
 * @{
 */
#define I2C_EVENT_ERROR               (1 << 1)
#define I2C_EVENT_ERROR_NO_SLAVE      (1 << 2)
#define I2C_EVENT_TRANSFER_COMPLETE   (1 << 3)
#define I2C_EVENT_TRANSFER_EARLY_NACK (1 << 4)
#define I2C_EVENT_ALL                 (I2C_EVENT_ERROR |  I2C_EVENT_TRANSFER_COMPLETE | I2C_EVENT_ERROR_NO_SLAVE | I2C_EVENT_TRANSFER_EARLY_NACK)

#define WRITE_ADDR (211 & 0xFE)
#define READ_ADDR (211 | 0x01)

/**@}*/

/** Non-asynch i2c hal structure
 */
typedef struct i2c_s i2c_t;

enum {
  I2C_ERROR_NO_SLAVE = -1,
  I2C_ERROR_BUS_BUSY = -2
};

#ifdef __cplusplus
extern "C" {
#endif

/**@}*/

#ifdef __cplusplus
}
#endif

#define I2C_CONSET(x)       (x->i2c->CONSET)
#define I2C_CONCLR(x)       (x->i2c->CONCLR)
#define I2C_STAT(x)         (x->i2c->STAT)
#define I2C_DAT(x)          (x->i2c->DAT)
#define I2C_SCLL(x, val)    (x->i2c->SCLL = val)
#define I2C_SCLH(x, val)    (x->i2c->SCLH = val)

static inline void i2c_conclr(i2c_t *obj, int start, int stop, int interrupt, int acknowledge) {
    I2C_CONCLR(obj) = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

static inline void i2c_conset(i2c_t *obj, int start, int stop, int interrupt, int acknowledge) {
    I2C_CONSET(obj) = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

// Clear the Serial Interrupt (SI)
static inline void i2c_clear_SI(i2c_t *obj) {
    i2c_conclr(obj, 0, 0, 1, 0);
}

static inline int i2c_status(i2c_t *obj) {
    return I2C_STAT(obj);
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI(i2c_t *obj) {
    int timeout = 0;
    while (!(I2C_CONSET(obj) & (1 << 3))) {
        timeout++;
        if (timeout > 100000) return -1;
    }
    return 0;
}

static inline void i2c_interface_enable(i2c_t *obj) {
    I2C_CONSET(obj) = 0x40;
}

static inline void i2c_power_enable(i2c_t *obj) {
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);
    LPC_SYSCON->PRESETCTRL |= 1 << 1;
}

static inline void i2c_init(i2c_t *obj) {
    // determine the SPI to use

    obj->i2c = (LPC_I2C_Type *)I2C_0;
    
    // enable power
    i2c_power_enable(obj);
    
    // set default frequency at 100k
    uint32_t PCLK = SystemCoreClock;
    uint32_t pulse = PCLK / (200000);
    I2C_SCLL(obj, pulse);
    I2C_SCLH(obj, pulse);

    i2c_conclr(obj, 1, 1, 1, 1);
    i2c_interface_enable(obj);

    __IO uint32_t *regSDA = (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * 5);
    __IO uint32_t *regSCL = (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * 4);

    *regSDA = ((*regSDA & ~0x7) | 0x1) & ~(0x3 << 3) & ~(0x1 << 10);
    *regSCL = ((*regSCL & ~0x7) | 0x1) & ~(0x3 << 3) & ~(0x1 << 10);
}

static inline int i2c_start(i2c_t *obj) {
    int status = 0;
    // 8.1 Before master mode can be entered, I2CON must be initialised to:
    //  - I2EN STA STO SI AA - -
    //  -  1    0   0   0  x - -
    // if AA = 0, it can't enter slave mode
    i2c_conclr(obj, 1, 1, 1, 1);
    
    // The master mode may now be entered by setting the STA bit
    // this will generate a start condition when the bus becomes free
    i2c_conset(obj, 1, 0, 0, 1);
    
    i2c_wait_SI(obj);
    status = i2c_status(obj);
    
    // Clear start bit now transmitted, and interrupt bit
    i2c_conclr(obj, 1, 0, 0, 0);
    return status;
}

static inline int i2c_stop(i2c_t *obj) {
    int timeout = 0;

    // write the stop bit
    i2c_conset(obj, 0, 1, 0, 0);
    i2c_clear_SI(obj);
    
    // wait for STO bit to reset
    while(I2C_CONSET(obj) & (1 << 4)) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    return 0;
}

static inline int i2c_do_write(i2c_t *obj, int value, uint8_t addr) {
    // write the data
    I2C_DAT(obj) = value;
    
    // clear SI to init a send
    i2c_clear_SI(obj);
    
    // wait and return status
    i2c_wait_SI(obj);
    return i2c_status(obj);
}

static inline int i2c_do_read(i2c_t *obj, int last) {
    // we are in state 0x40 (SLA+R tx'd) or 0x50 (data rx'd and ack)
    if (last) {
        i2c_conclr(obj, 0, 0, 0, 1); // send a NOT ACK
    } else {
        i2c_conset(obj, 0, 0, 0, 1); // send a ACK
    }
    
    // accept byte
    i2c_clear_SI(obj);
    
    // wait for it to arrive
    i2c_wait_SI(obj);
    
    // return the data
    return (I2C_DAT(obj) & 0xFF);
}

// The I2C does a read or a write as a whole operation
// There are two types of error conditions it can encounter
//  1) it can not obtain the bus
//  2) it gets error responses at part of the transmission
//
// We tackle them as follows:
//  1) we retry until we get the bus. we could have a "timeout" if we can not get it
//      which basically turns it in to a 2)
//  2) on error, we use the standard error mechanisms to report/debug
//
// Therefore an I2C transaction should always complete. If it doesn't it is usually
// because something is setup wrong (e.g. wiring), and we don't need to programatically
// check for that

static inline int i2c_read(i2c_t *obj, char *data, int length) {
    int count, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, READ_ADDR, 1);
    if (status != 0x40) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(obj, 0);
        status = i2c_status(obj);
        if (status != 0x50) {
            i2c_stop(obj);
            return count;
        }
        data[count] = (char) value;
    }

    // read in last byte
    int value = i2c_do_read(obj, 1);
    status = i2c_status(obj);
    if (status != 0x58) {
        i2c_stop(obj);
        return length - 1;
    }
    
    data[count] = (char) value;
    
    i2c_stop(obj);
    
    return length;
}

int i2c_write(i2c_t *obj, const char *data, int length) {
    int i, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, WRITE_ADDR, 1);
    if (status != 0x18) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    for (i=0; i<length; i++) {
        status = i2c_do_write(obj, data[i], 0);
        if(status != 0x28) {
            i2c_stop(obj);
            return i;
        }
    }
    
    // clearing the serial interrupt here might cause an unintended rewrite of the last byte
    // see also issue report https://mbed.org/users/mbed_official/code/mbed/issues/1
    // i2c_clear_SI(obj);
    
    i2c_stop(obj);
    
    return length;
}

int i2c_byte_write(i2c_t *obj, int data) {
    i2c_do_write(obj, (data & 0xFF), 0);

    return 0;
}


#endif

