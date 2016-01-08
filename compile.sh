#!/bin/sh

cd lib

INCLUDE="-I.. -I./api"

CPU="-mcpu=cortex-m0 -mthumb"
CC_FLAGS="$CPU -c -g -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP"
CC_SYMBOLS="-D__CORTEX_M0 -DTARGET_LPC11UXX -DTOOLCHAIN_GCC -DTARGET_LPC11U24_401 -DTARGET_CORTEX_M -DTARGET_NXP -DTOOLCHAIN_GCC_ARM -DTARGET_M0 -DARM_MATH_CM0 -DMBED_BUILD_TIMESTAMP=1447257386.83 -DTARGET_LPC11U24 -D__MBED__=1 -DNDEBUG -Os"
LD_FLAGS="$CPU -Wl,--gc-sections --specs=nano.specs -Wl,--wrap,main -Wl,-Map=i2c.map,--cref"
LD_SYS_LIBS="-lstdc++ -lsupc++ -lm -lc -lgcc -lnosys"
LINKER_SCRIPT="../LPC11U24.ld"

CC="arm-none-eabi-gcc $CC_FLAGS $CC_SYMBOLS -std=gnu99 $INCLUDE -o"

$CC gpio.o gpio.c
$CC board.o board.c
$CC i2c_api.o i2c_api.c
$CC pinmap.o pinmap.c
$CC serial_api.o serial_api.c
$CC us_ticker.o us_ticker.c
$CC gpio_api.o gpio_api.c
$CC pinmap_common.o pinmap_common.c
$CC mbed_interface.o mbed_interface.c
$CC wait_api.o wait_api.c
$CC semihost_api.o semihost_api.c
$CC us_ticker_api.o us_ticker_api.c
$CC ticker_api.o ticker_api.c
$CC system_LPC11Uxx.o system_LPC11Uxx.c
$CC cmsis_nvic.o cmsis_nvic.c
$CC PeripheralPins.o PeripheralPins.c

CPP="arm-none-eabi-g++ $CC_FLAGS $CC_SYMBOLS -std=gnu++98 -fno-rtti $INCLUDE -o"

$CPP I2C.o I2C.cpp
$CPP main.o ../main.cpp
$CPP classify.o ../classify.cpp
$CPP I2Cdev.o I2Cdev.cpp
$CPP MPU6050.o MPU6050.cpp
$CPP retarget.o retarget.cpp

arm-none-eabi-gcc $CPU -c -x assembler-with-cpp -o startup.o startup_LPC11xx.S

OBJECTS="gpio.o board.o i2c_api.o pinmap.o serial_api.o ticker_api.o gpio_api.o us_ticker.o pinmap_common.o us_ticker_api.o mbed_interface.o wait_api.o semihost_api.o system_LPC11Uxx.o cmsis_nvic.o PeripheralPins.o I2C.o main.o classify.o I2Cdev.o MPU6050.o retarget.o startup.o"

arm-none-eabi-gcc $LD_FLAGS -T$LINKER_SCRIPT -o out.elf $OBJECTS $LD_SYS_LIBS

arm-none-eabi-objcopy -O binary out.elf ../OUTPUT.bin
