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
const int32_t W[183] =
{
    14238,-50986,12262, 21435,215,-14647,   14523,-851,-6064,   6481,-11362,11551,  947,2781,-5745, -1220,4561,1895,    13619,-7657,3619,   -6704,6084,-9673,   -2749,7,7742,   6102,-5268,152, -1816,4680,-10736,  -351,-5644,10695,   -7681,-5363,-2014,  7202,-10117,-2042,  798,-4062,10682,    -7956,3666,-8807,   8204,-16602,3690,   -2329,-6566,10899,  -2405,13334,-9959,  8225,-1897,4588,    -4762,-3292,7500,   6103,-1439,-2165,   9530,-3382,6961,    -9109,4635,3602,    -3754,715,2852, 15168,-5643,3477,   -4877,8807,1858,    -6705,-1164,5455,   25913,-7483,5382,   8078,3037,6236, -8534,1508,6034,    21474,-11027,5515,  8943,-4673,6462,    -14044,4328,-553,   16894,-14415,6258,  1017,-8833,4344,    -12918,7876,-8343,  9399,-8868,2301,    1531,-5134,2649,    -9855,10614,-15675, 9257,-2860,-3557,   12496,-4052,2186,   -11872,6149,-12953, 9308,709,-9999, 17290,-9232,872,    -12334,10701,-7768, 2439,3489,-14930,   20619,-9054,-817,   -1141,8546,-12324,  1459,7205,-12629,   9642,-9412,-239,    -8822,1651,-6664,   887,3004,-10115,    -4723,-14326,-1759, -11441,-9260,-10588,    -3125,-172,-13768,  -5659,-12160,80,    5634,-10098,-6603,  -19547,-2730,-23169,    13206,-5338,6273,   -5935,-5196,997,

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
    int32_t nodeC = *offset++;
    int8_t j      = rear;

    do
    {
        j      = (j + 1) % BUFFER_SIZE;
        nodeA += (*offset++ * buffer[j]) >> 12;
        nodeB += (*offset++ * buffer[j]) >> 12;
        nodeC += (*offset++ * buffer[j]) >> 12;
    } while (j != rear);

    sigmoid(&nodeA);
    sigmoid(&nodeB);
    sigmoid(&nodeC);

    /*int32_t v[3];

    for (int32_t* i = v; i < v + 3; i++)
    {
        *i  = *offset++;
        *i += (*offset++ * nodeA) >> 12;
        *i += (*offset++ * nodeB) >> 12;
        sigmoid(i);
    }*/

    // Detects peak or trough
    if (nodeA > nodeC && nodeA > nodeB)
    {
        return PEAK;
    }
    else if (nodeB > nodeC)
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