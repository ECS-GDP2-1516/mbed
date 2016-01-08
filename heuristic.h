#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_

#define HEUR_SIZE 10
#define PEAK 0
#define TROU 1
#define NOT_EX 2

void init_heur();

int heur_classify(int value);

#endif
