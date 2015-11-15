#! /bin/sh

rm /media/mohit/MBED/mbed_blinky.bin
make clean
make
cp mbed_blinky.bin /media/mohit/MBED
