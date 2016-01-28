#ifndef _ASR_H_
#define _ASR_H_

#ifdef NO_ASR

#include <stdint.h>

/**
 * Software Implementation of Arithmetic Shift Right
 *
 * Performs a right shift on a given value, retaining the MSB to
 * ensure signed integers do not become corrupted. Used to approximate
 * division by 2 on systems without an ASRS intruction.
 *
 * @param int32_t val The value to shift
 * @param int8_t amount The shift to perform
 * @return int32_t The shifted value, with negative value preserved
 */
static int32_t asr(int32_t val, int8_t amount)
{
    int32_t msb = val & 0x80;
    
    return (val >> amount) | msb;
}

#else

/**
 * If the device supports ASR, simply use a bitshift and have
 * GCC create an ASRS instruction.
 */
#define asr(x, y)   (x) >> y

#endif

#endif /* _ASR_H_ */