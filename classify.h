#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#define BUFFER_SIZE 30

#include <stdint.h>

int classify(int8_t rear, int16_t buffer[]);

void sigmoid(int* var);

#endif