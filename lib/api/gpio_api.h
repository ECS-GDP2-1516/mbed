/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_GPIO_API_H
#define MBED_GPIO_API_H

#include "device.h"
#include "pinmap.h"
#include "mbed_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint32_t mask;

    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_set;
    __IO uint32_t *reg_clr;
    __I  uint32_t *reg_in;
} gpio_t;

static inline uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    int f = ((pin == P0_0)  ||
             (pin == P0_10) ||
             (pin == P0_11) ||
             (pin == P0_12) ||
             (pin == P0_13) ||
             (pin == P0_14) ||
             (pin == P0_15)) ? (1) : (0);
    
    pin_function(pin, f);
    
    return (1 << ((int)pin & 0x1F));
}

static inline void gpio_init(gpio_t *obj, PinName pin) {
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);
    
    unsigned int port = (unsigned int)pin >> PORT_SHIFT;
    
    obj->reg_set = &LPC_GPIO->SET[port];
    obj->reg_clr = &LPC_GPIO->CLR[port];
    obj->reg_in  = &LPC_GPIO->PIN[port];
    obj->reg_dir = &LPC_GPIO->DIR[port];
}

static inline void gpio_write(gpio_t *obj, int value) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    if (value)
        *obj->reg_set = obj->mask;
    else
        *obj->reg_clr = obj->mask;
}

static inline int gpio_read(gpio_t *obj) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    return ((*obj->reg_in & obj->mask) ? 1 : 0);
}

#ifdef __cplusplus
}
#endif

#endif
