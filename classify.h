#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#define BUFFER_SIZE 30

#include <stdint.h>

uint8_t classify(int8_t rear, int16_t buffer[]);

void sigmoid(int32_t* var);

#endif