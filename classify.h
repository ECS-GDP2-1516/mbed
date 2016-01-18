#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#define BUFFER_SIZE 60

#include <stdint.h>

/**
 * Approximates sigmoid with the given value
 */
static void sigmoid(int32_t* var);

/**
 * Classifications this MLP is capable of
 */
typedef enum
{
    PEAK   = 0,
    TROU   = 1,
    NOT_EX = 2
} ExType;

/**
 * Weights for the connections between the MLP neural network.
 *
 * This represent float which have been multiplied by 2^12 and then
 * rounded to the nearest int. As such, when multiplying two of these
 * together, remember to shift down by 12 afterwards.
 */
const int32_t W[131] =
{
    /* Input Layer: */
        /* Base: */
            /* Node A: */ -43403,
            /* Node B: */  30268,

        /* Scales: */
            /*      | Ax    | Bx    | Ay    | By    | Az    | Bz   */
            /* 01 */  464,    23676,  499,    8908,  -10668,  11246,
            /* 01 */  1482,   1298,   5179,  -565,   -9677,   16349,
            /* 01 */  5083,  -8960,   510,   -1656,  -6787,   3242,
            /* 01 */  5925,  -8889,  -3035,  -4796,  -5080,  -10290,
            /* 01 */  -7241,  5360,  -2785,  -1244,   1751,  -3528,
            /* 01 */  -14774, 9158,  -5371,   797,    10074, -313,
            /* 01 */  -3100,  11701, -2773,  -5160,   813,    3781,
            /* 01 */  -4060,  13962,  5938,  -10342,  2956,  -3331,
            /* 01 */  -6115,  20771,  9524,  -5828,   1338,  -2575,
            /* 01 */  -8448,  30821,  3333,   8536,   4551,  -9402,
            /* 01 */  -10063, 24610, -3425,   13005,  5505,  -18427,
            /* 01 */  -11923, 21109, -7792,   5357,   6195,  -21152,
            /* 01 */  -8048,  15546, -6207,   7038,   9076,  -20100,
            /* 01 */  -3547,  14209, -6307,   13832,  6578,  -17065,
            /* 01 */  -137,   14819, -9717,   20252,  9355,  -14432,
            /* 01 */  2341,   5250,  -8438,   23385,  5112,  -2703,
            /* 01 */  6296,   3696,  -8911,   11275, -658,   -14108,
            /* 01 */  3277,  -1036,  -13273, -6731,  -6120,  -16144,
            /* 01 */  1310,  -8152,  -13371, -7781,  -8820,   463,
            /* 01 */  -936,  -20966, -13463,  1216,   -6256, -14997,

    /* Output Layer: */
        /*              Base   | Node A | Node B */
        /* EX_PEAK: */ -21807,  -27118,   34204,
        /* EX_TROU: */ -20642,   37124,  -22091,
        /* NOT_EX:  */  22397,  -39003,  -34865
};

/**
 * Classifies the data in the given buffer using the hardcoded neural network
 *
 * @param int8_t rear The index of the last element in the buffer
 * @param int16_t[] buffer A circular buffer
 * @return ExType The classified form of movement
 */
static inline ExType classify(int8_t rear, int16_t buffer[])
{
    int32_t* offset=(int32_t*)W;
    
    int32_t nodeA = *offset++;
    int32_t nodeB = *offset++;
    int8_t j      = rear;

    do
    {
        j      = (j + 1) % BUFFER_SIZE;
        nodeA += (*offset++ * buffer[j]) >> 12;
        nodeB += (*offset++ * buffer[j]) >> 12;
    } while (j != rear);

    sigmoid(&nodeA);
    sigmoid(&nodeB);

    int32_t v[3];

    for (int32_t* i = v; i < v + 3; i++)
    {
        *i  = *offset++;
        *i += (*offset++ * nodeA) >> 12;
        *i += (*offset++ * nodeB) >> 12;
        sigmoid(i);
    }

    // Detects peak or trough
    if (v[0] > v[2] && v[0] > v[1])
    {
        return PEAK;
    }
    else if (v[1] > v[2])
    {
        return TROU;
    }
    else //Not exercise
    {
        return NOT_EX;
    }
}

/**
 * Approximates Sigmoid for the given value
 *
 * @param int32_t* var Pointer to an int value
 */
static void sigmoid(int32_t* var)
{
    if (*var < -5 << 12)
    {
        *var = 0;
    }
    else if (*var > 5 << 12)
    {
        *var = 1 << 12;
    }
    else
    {
        *var = ((410 * *var) >> 12) + 2048;
    }
}

#endif