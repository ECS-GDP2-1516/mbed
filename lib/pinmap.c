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
#include "mbed_assert.h"
#include "pinmap.h"

void pin_function(PinName pin, int function) {
    MBED_ASSERT(pin != (PinName)NC);
    if (pin == (PinName)NC) return;
    
    uint32_t pin_number = (uint32_t)pin;
    
    __IO uint32_t *reg = (pin_number < 32) ?
                         (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * pin_number) :
                         (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));
    
    // pin function bits: [2:0] -> 111 = (0x7)
    *reg = (*reg & ~0x7) | (function & 0x7);
}
