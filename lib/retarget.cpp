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
#include "platform.h"
#include "serial_api.h"
#include "toolchain.h"
#include "mbed_interface.h"
#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif
#include <errno.h>

#if defined(__ARMCC_VERSION)
#   include <rt_sys.h>
#   define PREFIX(x)    _sys##x
#   define OPEN_MAX     _SYS_OPEN
#   ifdef __MICROLIB
#       pragma import(__use_full_stdio)
#   endif

#elif defined(__ICCARM__)
#   include <yfuns.h>
#   define PREFIX(x)        _##x
#   define OPEN_MAX         16

#   define STDIN_FILENO     0
#   define STDOUT_FILENO    1
#   define STDERR_FILENO    2

#else
#   include <sys/stat.h>
#   include <sys/unistd.h>
#   include <sys/syslimits.h>
#   define PREFIX(x)    x
#endif

//using namespace mbed;

#if defined(__MICROLIB) && (__ARMCC_VERSION>5030000)
// Before version 5.03, we were using a patched version of microlib with proper names
extern const char __stdin_name[]  = ":tt";
extern const char __stdout_name[] = ":tt";
extern const char __stderr_name[] = ":tt";

#else
extern const char __stdin_name[]  = "/stdin";
extern const char __stdout_name[] = "/stdout";
extern const char __stderr_name[] = "/stderr";
#endif


// ****************************************************************************
// mbed_main is a function that is called before main()
// mbed_sdk_init() is also a function that is called before main(), but unlike
// mbed_main(), it is not meant for user code, but for the SDK itself to perform
// initializations before main() is called.

extern "C" WEAK void mbed_main(void);
extern "C" WEAK void mbed_main(void) {
}

extern "C" WEAK void mbed_sdk_init(void);
extern "C" WEAK void mbed_sdk_init(void) {
}

#if defined(TOOLCHAIN_ARM)
extern "C" int $Super$$main(void);

extern "C" int $Sub$$main(void) {
    mbed_sdk_init();
    mbed_main();
    return $Super$$main();
}
#elif defined(TOOLCHAIN_GCC)
extern "C" int __real_main(void);

extern "C" int __wrap_main(void) {
    mbed_sdk_init();
    mbed_main();
    return __real_main();
}
#elif defined(TOOLCHAIN_IAR)
// IAR doesn't have the $Super/$Sub mechanism of armcc, nor something equivalent
// to ld's --wrap. It does have a --redirect, but that doesn't help, since redirecting
// 'main' to another symbol looses the original 'main' symbol. However, its startup
// code will call a function to setup argc and argv (__iar_argc_argv) if it is defined.
// Since mbed doesn't use argc/argv, we use this function to call our mbed_main.
extern "C" void __iar_argc_argv() {
    mbed_sdk_init();
    mbed_main();
}
#endif

// Provide implementation of _sbrk (low-level dynamic memory allocation
// routine) for GCC_ARM which compares new heap pointer with MSP instead of
// SP.  This make it compatible with RTX RTOS thread stacks.
#if defined(TOOLCHAIN_GCC_ARM)
// Linker defined symbol used by _sbrk to indicate where heap should start.
extern "C" int __end__;

#if defined(TARGET_CORTEX_A)
extern "C" uint32_t  __HeapLimit;
#endif

// Turn off the errno macro and use actual global variable instead.
#undef errno
extern "C" int errno;

// For ARM7 only
register unsigned char * stack_ptr __asm ("sp");

// Dynamic memory allocation related syscall.
extern "C" caddr_t _sbrk(int incr) {
    static unsigned char* heap = (unsigned char*)&__end__;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

#if defined(TARGET_ARM7)
    if (new_heap >= stack_ptr) {
#elif defined(TARGET_CORTEX_A)
    if (new_heap >= (unsigned char*)&__HeapLimit) {     /* __HeapLimit is end of heap section */
#else
    if (new_heap >= (unsigned char*)__get_MSP()) {
#endif
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
#endif

/*
#ifdef TOOLCHAIN_GCC_CW
// TODO: Ideally, we would like to define directly "_ExitProcess"
extern "C" void mbed_exit(int return_code) {
#elif defined TOOLCHAIN_GCC_ARM
extern "C" void _exit(int return_code) {
#else
namespace std {
extern "C" void exit(int return_code) {
#endif

#if DEVICE_STDIO_MESSAGES
    fflush(stdout);
    fflush(stderr);
#endif

#if DEVICE_SEMIHOST
    if (mbed_interface_connected()) {
        semihost_exit();
    }
#endif
    if (return_code) {
        mbed_die();
    }

    while (1);
}
*/

#if !defined(TOOLCHAIN_GCC_ARM) && !defined(TOOLCHAIN_GCC_CW)
} //namespace std
#endif


namespace mbed {

void mbed_set_unbuffered_stream(FILE *_file) {
#if defined (__ICCARM__)
    char buf[2];
    std::setvbuf(_file,buf,_IONBF,NULL);    
#else
    setbuf(_file, NULL);
#endif
}

int mbed_getc(FILE *_file){
#if defined (__ICCARM__)
    /*This is only valid for unbuffered streams*/
    int res = std::fgetc(_file);
    if (res>=0){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }    
    return res;
#else    
    return std::fgetc(_file);
#endif   
}

char* mbed_gets(char*s, int size, FILE *_file){
#if defined (__ICCARM__)
    /*This is only valid for unbuffered streams*/
    char *str = fgets(s,size,_file);
    if (str!=NULL){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }
    return str;
#else    
    return std::fgets(s,size,_file);
#endif
}

} // namespace mbed
