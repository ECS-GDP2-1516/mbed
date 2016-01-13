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

#include "LPC11Uxx.h"


typedef struct {                            /*!< (@ 0x40000000) I2C Structure          */
  __IO uint32_t CONSET;                     /*!< (@ 0x40000000) I2C Control Set Register */
  __I  uint32_t STAT;                       /*!< (@ 0x40000004) I2C Status Register */
  __IO uint32_t DAT;                        /*!< (@ 0x40000008) I2C Data Register.  */
  __IO uint32_t ADR0;                       /*!< (@ 0x4000000C) I2C Slave Address Register 0 */
  __IO uint32_t SCLH;                       /*!< (@ 0x40000010) SCH Duty Cycle Register High Half Word */
  __IO uint32_t SCLL;                       /*!< (@ 0x40000014) SCL Duty Cycle Register Low Half Word */
  __IO uint32_t CONCLR;                     /*!< (@ 0x40000018) I2C Control Clear Register*/
  __IO uint32_t MMCTRL;                     /*!< (@ 0x4000001C) Monitor mode control register*/
  __IO uint32_t ADR1;                       /*!< (@ 0x40000020) I2C Slave Address Register 1*/
  __IO uint32_t ADR2;                       /*!< (@ 0x40000024) I2C Slave Address Register 2*/
  __IO uint32_t ADR3;                       /*!< (@ 0x40000028) I2C Slave Address Register 3*/
  __I  uint32_t DATA_BUFFER;                /*!< (@ 0x4000002C) Data buffer register */
union{
  __IO uint32_t MASK[4];                    /*!< (@ 0x40000030) I2C Slave address mask register */
  struct{
  __IO uint32_t MASK0;
  __IO uint32_t MASK1;
  __IO uint32_t MASK2;
  __IO uint32_t MASK3;
  };
  };
} LPC_I2C_Type;

enum {
  I2C_ERROR_NO_SLAVE = -1,
  I2C_ERROR_BUS_BUSY = -2
};

#define WRITE_ADDR (211 & 0xFE)
#define READ_ADDR  (211 | 0x01)

#define SDA_REG ((__IO uint32_t*)(LPC_IOCON0_BASE + 4 * 5))
#define SCL_REG ((__IO uint32_t*)(LPC_IOCON0_BASE + 4 * 4))

#define PULSE (__SYSTEM_CLOCK / 200000)

#define LPC_I2C_BASE              (0x40000000)
#define LPC_I2C                   ((LPC_I2C_Type            *) LPC_I2C_BASE)

static inline void i2c_conclr(int start, int stop, int interrupt, int acknowledge) {
    LPC_I2C->CONCLR = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

static inline void i2c_conset(int start, int stop, int interrupt, int acknowledge) {
    LPC_I2C->CONSET = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

// Clear the Serial Interrupt (SI)
static inline void i2c_clear_SI() {
    i2c_conclr(0, 0, 1, 0);
}

static inline int i2c_status() {
    return LPC_I2C->STAT;
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI() {
    int timeout = 0;
    while (!(LPC_I2C->CONSET & (1 << 3))) {
        timeout++;
        if (timeout > 100000) return -1;
    }
    return 0;
}

static inline void i2c_interface_enable() {
    LPC_I2C->CONSET = 0x40;
}

static inline void i2c_power_enable() {
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);
    LPC_SYSCON->PRESETCTRL |= 1 << 1;
}

static inline void i2c_init() {
    // determine the SPI to use
    
    // enable power
    i2c_power_enable();
    
    LPC_I2C->SCLL = PULSE;
    LPC_I2C->SCLH = PULSE;

    i2c_conclr(1, 1, 1, 1);
    i2c_interface_enable();

    *SDA_REG = (*SDA_REG & ~0x7) | 0x1;
    uint32_t tmp = *SDA_REG & ~(0x3 << 3) & ~(0x1 << 10);
    *SDA_REG = tmp;

    *SCL_REG = (*SCL_REG & ~0x7) | 0x1;
    tmp = *SCL_REG & ~(0x3 << 3) & ~(0x1 << 10);
    *SCL_REG = tmp;
}

static inline int i2c_start() {
    int status = 0;
    // 8.1 Before master mode can be entered, I2CON must be initialised to:
    //  - I2EN STA STO SI AA - -
    //  -  1    0   0   0  x - -
    // if AA = 0, it can't enter slave mode
    i2c_conclr(1, 1, 1, 1);
    
    // The master mode may now be entered by setting the STA bit
    // this will generate a start condition when the bus becomes free
    i2c_conset(1, 0, 0, 1);
    
    i2c_wait_SI();
    status = i2c_status();
    
    // Clear start bit now transmitted, and interrupt bit
    i2c_conclr(1, 0, 0, 0);
    return status;
}

static inline int i2c_stop() {
    int timeout = 0;

    // write the stop bit
    i2c_conset(0, 1, 0, 0);
    i2c_clear_SI();
    
    // wait for STO bit to reset
    while(LPC_I2C->CONSET & (1 << 4)) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    return 0;
}

static inline int i2c_do_write(int value) {
    // write the data
    LPC_I2C->DAT = value;
    
    // clear SI to init a send
    i2c_clear_SI();
    
    // wait and return status
    i2c_wait_SI();
    return i2c_status();
}

static inline int i2c_do_read(int last) {
    // we are in state 0x40 (SLA+R tx'd) or 0x50 (data rx'd and ack)
    if (last) {
        i2c_conclr(0, 0, 0, 1); // send a NOT ACK
    } else {
        i2c_conset(0, 0, 0, 1); // send a ACK
    }
    
    // accept byte
    i2c_clear_SI();
    
    // wait for it to arrive
    i2c_wait_SI();
    
    // return the data
    return (LPC_I2C->DAT & 0xFF);
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

static inline int i2c_read(char *data, int length) {
    int count, status;
    
    status = i2c_start();
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop();
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(READ_ADDR);
    if (status != 0x40) {
        i2c_stop();
        return I2C_ERROR_NO_SLAVE;
    }

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(0);
        status = i2c_status();
        if (status != 0x50) {
            i2c_stop();
            return count;
        }
        data[count] = (char) value;
    }

    // read in last byte
    int value = i2c_do_read(1);
    status = i2c_status();
    if (status != 0x58) {
        i2c_stop();
        return length - 1;
    }
    
    data[count] = (char) value;
    
    i2c_stop();
    
    return length;
}

int i2c_write(const char *data, int length) {
    int i, status;
    
    status = i2c_start();
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop();
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(WRITE_ADDR);
    if (status != 0x18) {
        i2c_stop();
        return I2C_ERROR_NO_SLAVE;
    }
    
    for (i=0; i<length; i++) {
        status = i2c_do_write(data[i]);
        if(status != 0x28) {
            i2c_stop();
            return i;
        }
    }
    
    // clearing the serial interrupt here might cause an unintended rewrite of the last byte
    // see also issue report https://mbed.org/users/mbed_official/code/mbed/issues/1
    // i2c_clear_SI(obj);
    
    i2c_stop();
    
    return length;
}




#endif

