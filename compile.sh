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
 --specs=nano.specs -mthumb -mcpu=cortex-m0 \
-D NDEBUG \
../fs/fat/FATFileSystem.cpp \
../fs/fat/FATFileHandle.cpp \
common/retarget.cpp \
common/I2C.cpp \
common/SerialBase.cpp \
common/Serial.cpp \
common/Stream.cpp \
common/FileBase.cpp \
common/FileLike.cpp \
common/FilePath.cpp \
common/gpio.c \
common/board.c \
common/wait_api.c \
common/semihost_api.c \
common/mbed_interface.c \
common/us_ticker_api.c \
common/ticker_api.c \
../../main.cpp \
../../classify.cpp \
../../I2Cdev.cpp \
../../MPU6050.cpp \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/serial_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/gpio_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/i2c_api.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/pinmap.c \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/us_ticker.c \
common/pinmap_common.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/system_LPC11Uxx.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/cmsis_nvic.c \
targets/cmsis/TARGET_NXP/TARGET_LPC11UXX/TOOLCHAIN_GCC_CS/sys.cpp \
targets/hal/TARGET_NXP/TARGET_LPC11UXX/TARGET_LPC11U24_401/PeripheralPins.c \
-fdata-sections -ffunction-sections \
-Wl,--gc-sections \
-o ../../i2c.out

cd ../..

arm-none-eabi-objcopy -O binary i2c.out i2c.bin
