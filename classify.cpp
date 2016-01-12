#include "classify.h"
#include "heuristic.h"

const int32_t W[71] = {779,-302,-5398,-994,-3456,-7674,-3501,8029,7970,9422,-8146,9542,-5343,-12618,-7811,-8860,-11129,-3766,11986,-4548,2397,14550,1399,-4294,-5781,-26540,-4584,-16282,5057,-7924,17022,
-78257,-20265,-40944,-20894,4222,2843,-1053,-9851,-7594,10020,-6371,-20675,20497,-11065,-8887,5446,-8741,6535,4986,-23657,-24732,8705,-19445,-50337,-3757,7458,-39086,8324,-402,-3564,1897,
2665,-49788,34207,
-63780,51797,22185,
-1993,36688,-45645,
};

int classify(int8_t rear, int16_t buffer[])
{
    int* offset=(int*)W;
    
    int nodeA = *offset++;
    int j     = rear;
    while (1)
    {
        j      = (j + 1) % BUFFER_SIZE;
        nodeA += (*offset++ * buffer[j]) >> 12;

        if (j == rear)
        {
            break;
        }
    }
    sigmoid(&nodeA);

    int nodeB = *offset++;
    j         = rear;
    while (1)
    {
        j      = (j + 1) % BUFFER_SIZE;
        nodeB += (*offset++ * buffer[j]) >> 12;

        if (j == rear)
        {
            break;
        }
    }
    sigmoid(&nodeB);

    int v[3];

    for (int* i = v; i < v + 3; i++)
    {
        *i  = *offset++;
        *i += (*offset++ * nodeA) >> 12;
        *i += (*offset++ * nodeB) >> 12;
        sigmoid(i);
    }

    // Detects peak or trough
    if (v[0] > v[2])
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

void sigmoid(int* var)
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
