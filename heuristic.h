#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_

#include "classify.h"

#define HEUR_SIZE 20
#define MAX_NOT_EX 5

int8_t heur_rear = -1;
uint8_t heur_buffer[HEUR_SIZE];

static inline void init_heur()
{
    for(uint8_t i = 0; i<HEUR_SIZE; i++)
    {
        heur_buffer[i] = NOT_EX;
    }
}

static inline void insert_value(uint8_t value)
{
    heur_rear              = (heur_rear + 1) % HEUR_SIZE;
    heur_buffer[heur_rear] = value;
}

static inline bool heur_classify(uint8_t value)
{
    insert_value(value);
    uint8_t not_ex = 0;
    for(uint8_t i = 0; i < HEUR_SIZE; i++)
    {
        if(heur_buffer[i] == NOT_EX)
        {
            not_ex++;
            if(not_ex > MAX_NOT_EX)
            {
                return 0;
            }
        }
    }
    return 1;
}

#endif
