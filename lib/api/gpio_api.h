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

#include "PinNames.h"

#define LPC_GPIO_REG_DIR ((uint32_t*)(0x50002004))
#define LPC_GPIO_REG_SET ((uint32_t*)(0x50002204))
#define LPC_GPIO_REG_CLR ((uint32_t*)(0x50002284))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t mask;
} gpio_t;

static inline uint32_t gpio_set(PinName pin) {
    uint32_t pin_number = (uint32_t)pin;
    __IO uint32_t *reg = (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));
    
    // pin function bits: [2:0] -> 111 = (0x7)
    *reg = (*reg & ~0x7);
    
    return (1 << ((int)pin & 0x1F));
}

static inline void gpio_init(gpio_t *obj, PinName pin) {
    obj->mask = gpio_set(pin);
}

static inline void gpio_write(gpio_t *obj, int value) {
    if (value)
        *LPC_GPIO_REG_SET = obj->mask;
    else
        *LPC_GPIO_REG_CLR = obj->mask;
}

#ifdef __cplusplus
}
#endif

#endif
