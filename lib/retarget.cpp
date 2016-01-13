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
#include "LPC11Uxx.h"
#include "toolchain.h"

#include <errno.h>

#   include <sys/unistd.h>

// ****************************************************************************
// mbed_main is a function that is called before main()
// mbed_sdk_init() is also a function that is called before main(), but unlike
// mbed_main(), it is not meant for user code, but for the SDK itself to perform
// initializations before main() is called.

extern "C" int __real_main(void);

extern "C" int __wrap_main(void) {
    //mbed_sdk_init();
    //mbed_main();
    return __real_main();
}

// Provide implementation of _sbrk (low-level dynamic memory allocation
// routine) for GCC_ARM which compares new heap pointer with MSP instead of
// SP.  This make it compatible with RTX RTOS thread stacks.
// Linker defined symbol used by _sbrk to indicate where heap should start.
extern "C" int __end__;

// Turn off the errno macro and use actual global variable instead.
#undef errno
extern "C" int errno;

// Dynamic memory allocation related syscall.
extern "C" caddr_t _sbrk(int incr) {
    static unsigned char* heap = (unsigned char*)&__end__;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

    if (new_heap >= (unsigned char*)__get_MSP()) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}


