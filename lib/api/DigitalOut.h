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
#ifndef MBED_DIGITALOUT_H
#define MBED_DIGITALOUT_H

 #include "LPC11Uxx.h"

#define LPC_GPIO_REG_DIR ((uint32_t*)(0x50002004))
#define LPC_GPIO_REG_SET ((uint32_t*)(0x50002204))
#define LPC_GPIO_REG_CLR ((uint32_t*)(0x50002284))

#define LED1_MASK (1 << (40 & 0x1F))
#define LED2_MASK (1 << (41 & 0x1F))
#define LED3_MASK (1 << (42 & 0x1F))
#define LED4_MASK (1 << (43 & 0x1F))

#define LED1_REG ((__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (40 - 32)))
#define LED2_REG ((__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (41 - 32)))
#define LED3_REG ((__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (42 - 32)))
#define LED4_REG ((__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (43 - 32)))

void init_led(__IO uint32_t* reg, uint32_t mask)
{
    *reg               = (*reg & ~0x7);
    *LPC_GPIO_REG_CLR  = mask;;
    *LPC_GPIO_REG_DIR |= mask;
    
    uint32_t tmp = *reg;
    tmp &= ~(0x3 << 3);
    tmp &= ~(0x1 << 10);
    *reg = tmp;
}

static inline void led_on(uint32_t mask)
{
    *LPC_GPIO_REG_SET = mask;
}

static inline void led_off(uint32_t mask)
{
    *LPC_GPIO_REG_CLR = mask;
}

#endif
