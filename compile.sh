#!/bin/sh

cd libraries/mbed

arm-none-eabi-g++ \
-I ../.. \
-I ../../mbed \
-I ./api \
-I ./hal \
-I ./hal/TARGET_NXP/TARGET_LPC11UXX \
-I ./hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401 \
-I ./targets/hal/TARGET_NXP/TARGET_LPC11UXX \
-I ./targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401 \
-I ./targets/cmsis/TARGET_NXP/TARGET_LPC11UXX \
-I ./targets/cmsis \
-Os -s -fno-common -fdata-sections -ffunction-sections \
--specs=nano.specs -mthumb -mcpu=cortex-m0 -g -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP \
-D NDEBUG \
common/I2C.cpp \
common/gpio.c \
../../main.cpp \
../../classify.cpp \
../../I2Cdev.cpp \
../../MPU6050.cpp \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/gpio_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/i2c_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/pinmap.c \
common/pinmap_common.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/system_LPC11Uxx.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/TOOLCHAIN_GCC_CS/sys.cpp \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401/PeripheralPins.c \
-Wl,--gc-sections \
-o ../../i2c.elf

cd ../..

arm-none-eabi-objcopy -O binary i2c.elf i2c.bin
