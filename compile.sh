#!/bin/sh

cd libraries/mbed

INCLUDE="-I ../.. -I ../../mbed -I ./api -I ./hal -I ./hal/TARGET_NXP/TARGET_LPC11UXX -I ./hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401 -I ./targets/hal/TARGET_NXP/TARGET_LPC11UXX -I ./targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401 -I ./targets/cmsis/TARGET_NXP/TARGET_LPC11UXX -I ./targets/cmsis"

CPU="-mcpu=cortex-m0 -mthumb"
CC_FLAGS="$CPU -c -g -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP"
CC_SYMBOLS="-D__CORTEX_M0 -DTARGET_LPC11UXX -DTOOLCHAIN_GCC -DTARGET_LPC11U24_401 -DTARGET_CORTEX_M -DTARGET_NXP -DTOOLCHAIN_GCC_ARM -DTARGET_M0 -DARM_MATH_CM0 -DMBED_BUILD_TIMESTAMP=1447257386.83 -DTARGET_LPC11U24 -D__MBED__=1 -DNDEBUG -Os"
LD_FLAGS="$CPU -Wl,--gc-sections --specs=nano.specs -Wl,--wrap,main -Wl,-Map=i2c.map,--cref"
LD_SYS_LIBS="-lstdc++ -lsupc++ -lm -lc -lgcc -lnosys"
LIBRARIES="-lmbed"
LINKER_SCRIPT="../../mbed/TARGET_LPC11U24/TOOLCHAIN_GCC_ARM/LPC11U24.ld"
LIBRARY_PATHS="-L../../mbed/TARGET_LPC11U24/TOOLCHAIN_GCC_ARM"

CC="arm-none-eabi-gcc $CC_FLAGS $CC_SYMBOLS -std=gnu99 $INCLUDE -o"

$CC gpio.o common/gpio.c
$CC board.o common/board.c
$CC gpio_api.o targets/hal/TARGET_NXP/TARGET_LPC11UXX/i2c_api.c
$CC pinmap.o targets/hal/TARGET_NXP/TARGET_LPC11UXX/pinmap.c
$CC pinmap_common.o common/pinmap_common.c
$CC system_LPC11Uxx.o targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/system_LPC11Uxx.c
$CC cmsis_nvic.o targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/cmsis_nvic.c
$CC PeripheralPins.o targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401/PeripheralPins.c

CPP="arm-none-eabi-g++ $CC_FLAGS $CC_SYMBOLS -std=gnu++98 -fno-rtti $INCLUDE -o"

$CPP I2C.o common/I2C.cpp
$CPP main.o ../../main.cpp
$CPP classify.o ../../classify.cpp
$CPP I2Cdev.o ../../I2Cdev.cpp
$CPP MPU6050.o ../../MPU6050.cpp
$CPP retarget.o common/retarget.cpp

arm-none-eabi-gcc $CPU -c -x assembler-with-cpp -o startup.o targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/TOOLCHAIN_GCC_ARM/startup_LPC11xx.S

OBJECTS="gpio.o board.o gpio_api.o pinmap.o pinmap_common.o system_LPC11Uxx.o cmsis_nvic.o PeripheralPins.o I2C.o main.o classify.o I2Cdev.o MPU6050.o retarget.o startup.o"

arm-none-eabi-gcc $LD_FLAGS -T$LINKER_SCRIPT $LIBRARY_PATHS -o out.elf $OBJECTS $LIBRARIES $LD_SYS_LIBS $LIBRARIES $LD_SYS_LIBS

arm-none-eabi-objcopy -O binary out.elf ../../OUTPUT.bin
