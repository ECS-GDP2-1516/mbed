#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_

#include <stdint.h>

#define HEUR_SIZE 10
#define PEAK 0
#define TROU 1
#define NOT_EX 2

void init_heur();

bool heur_classify(uint8_t value);

#endif
