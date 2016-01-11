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
#include "I2C.h"
#include "MPU6050.h"


#if DEVICE_I2C

namespace mbed {

I2C *I2C::_owner = NULL;

I2C::I2C(PinName sda, PinName scl) : _i2c(), _hz(100000) {
    // The init function also set the frequency to 100000
    i2c_init(&_i2c, sda, scl);

    // Used to avoid unnecessary frequency updates
    _owner = this;
}

void I2C::frequency(int hz) {
    _hz = hz;

    // We want to update the frequency even if we are already the bus owners
    i2c_frequency(&_i2c, _hz);

    // Updating the frequency of the bus we become the owners of it
    _owner = this;
}

void I2C::aquire() {
    if (_owner != this) {
        i2c_frequency(&_i2c, _hz);
        _owner = this;
    }
}

// write - Master Transmitter Mode
int I2C::write(const char* data, int length, bool repeated) {
    aquire();

    int stop = (repeated) ? 0 : 1;
    int written = i2c_write(&_i2c, MPU6050_DEFAULT_ADDRESS, data, length, stop);

    return length != written;
}

// read - Master Reciever Mode
int I2C::read(char* data, int length, bool repeated) {
    aquire();

    int stop = (repeated) ? 0 : 1;
    int read = i2c_read(&_i2c, MPU6050_DEFAULT_ADDRESS, data, length, stop);

    return length != read;
}

} // namespace mbed

#endif
