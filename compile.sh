#! /bin/sh

rm /media/mohit/MBED/mbed_blinky.bin
make
cp mbed_blinky.bin /media/mohit/MBED
make clean
