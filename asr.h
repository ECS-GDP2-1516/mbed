#ifndef _ASR_H_
#define _ASR_H_

#ifdef NO_ASR

#include <stdint.h>

static int32_t asr(int32_t val, int8_t amount)
{
    int32_t msb = val & 0x80;
    
    return (val >> amount) | msb;
}

#else

#define asr(x, y)   (x) >> y

#endif

#endif /* _ASR_H_ */