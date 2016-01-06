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
-I ../fs/fat/ChaN \
-Os -s -fno-common  \
-lc_nano -lrdimon --specs=nosys.specs -mthumb -mcpu=cortex-m0 \
-D NDEBUG \
../fs/fat/FATFileSystem.cpp \
../fs/fat/FATFileHandle.cpp \
common/I2C.cpp \
common/SerialBase.cpp \
common/Serial.cpp \
common/Stream.cpp \
common/FileBase.cpp \
common/FileLike.cpp \
common/gpio.c \
../../main.cpp \
../../classify.cpp \
../../I2Cdev.cpp \
../../MPU6050.cpp \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/serial_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/gpio_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/i2c_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/pinmap.c \
common/pinmap_common.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/system_LPC11Uxx.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401/PeripheralPins.c \
-fdata-sections -ffunction-sections \
-Wl,--gc-sections \
-o ../../i2c.out
