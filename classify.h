#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#define BUFFER_SIZE 30

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
const int32_t W[71] =
{
    /* Input Layer: */
        /* Base: */
            /* Node A: */  779,
            /* Node B: */ -78257,

        /* Scales: */
            /*        Ax |    Bx |    Ay |    By |    Az |    Bz */
            /* 01 */ -302,   -20265, -5398,  -40944, -994,   -20894,
            /* 02 */ -3456,   4222,  -7674,   2843,  -3501,  -1053,
            /* 03 */  8029,  -9851,   7970,  -7594,   9422,   10020,
            /* 04 */ -8146,  -6371,   9542,  -20675, -5343,   20497,
            /* 05 */ -12618, -11065, -7811,  -8887,  -8860,   5446,
            /* 06 */ -11129, -8741,  -3766,   6535,   11986,  4986,
            /* 07 */ -4548,  -23657,  2397,  -24732,  14550,  8705,
            /* 08 */  1399,  -19445, -4294,  -50337, -5781,  -3757,
            /* 09 */ -26540,  7458,  -4584,  -39086, -16282,  8324,
            /* 10 */  5057,  -402,   -7924,  -3564,   17022,  1897,
        

    /* Output Layer: */
        /*              Base  | Node A | Node B */
        /* EX_PEAK: */  2665,  -49788,   34207,
        /* EX_TROU: */ -63780,  51797,   22185,
        /* NOT_EX:  */ -1993,   36688,  -45645
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