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
            /* Node A: */ -48651,
            /* Node B: */  28586,

    /* Scales: */
            /*      | Ax    | Bx    | Ay    | By    | Az    | Bz   */
            /* 01 */  1893,   33384, -2339,   13700, -13204, 13289,
            /* 02 */  1294,  -94,     2490,  -751,   -5823,  11298,
            /* 03 */  3487,  -13372,  1051,  -6754,  -5594,  5577,
            /* 04 */  4217,  -872,   -2099,  -2841,  -4064, -10162,
            /* 05 */ -6499,   13436, -2127,   5197,   5671, -11623,
            /* 06 */ -12920,  11861, -3718,   4449,   12239, 1306,
            /* 07 */ -2425,   10297, -1681,   1636,  -2736,  10602,
            /* 08 */ -3988,   12709,  4296,  -11206,  212,  -8106,
            /* 09 */ -5047,   22551,  5271,  -11641,  989,  -8604,
            /* 10 */ -6897,   35625, -1091,   9829,   1192, -8218,
            /* 11 */ -9593,   31595, -3962,   8071,   3443, -21547,
            /* 12 */ -11884,  23170, -5169,  -1237,   6474, -22817,
            /* 13 */ -8670,   9011,  -4208,   1462,   8572, -10896,
            /* 14 */ -4098,   9655,  -5843,   16551,  7360, -11930,
            /* 15 */ -744,    12505, -9735,   27964,  9793, -15546,
            /* 16 */  1282,   3199,  -8819,   33574,  6967, -4160,
            /* 17 */  4749,   207,   -9980,   14540,  1453, -18561,
            /* 18 */  2568,  -558,   -12585, -11228, -8089, -19157,
            /* 19 */  209,   -3815,  -10045, -11079, -9359,  16232,
            /* 20 */ -2770,  -30365, -5101,   13555, -3687, -8294,

    /* Output Layer: */
        /*              Base   | Node A | Node B */
        /* EX_PEAK: */ -20952,  -20508,   32406,
        /* EX_TROU: */ -20403,   37879,  -15746,
        /* NOT_EX:  */  21273,  -38453,  -32719
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