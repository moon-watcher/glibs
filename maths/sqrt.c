#include <stdint.h>

uint32_t sqrt(uint32_t x)
{
    uint32_t res = 0;
    uint32_t bit = 1UL << 30;

    while (bit > x)
        bit >>= 2;

    while (bit)
    {
        uint32_t temp = res + bit;

        if (x < temp)
            res >>= 1;
        else
        {
            x -= temp;
            res = (res >> 1) + bit;
        }

        bit >>= 2;
    }

    return res;
}

uint16_t sqrt16(uint16_t x)
{
    uint16_t res = 0;
    uint16_t bit;

         if (x >= 16384) bit = 1 << 14;
    else if (x >=  4096) bit = 1 << 12;
    else if (x >=  1024) bit = 1 << 10;
    else if (x >=   256) bit = 1 <<  8;
    else if (x >=    64) bit = 1 <<  6;
    else if (x >=    16) bit = 1 <<  4;
    else if (x >=     4) bit = 1 <<  2;
    else                 bit = 1 <<  0;

    while (bit)
    {
        uint16_t temp = res + bit;

        if (x < temp)
            res >>= 1;
        else
        {
            x -= temp;
            res = (res >> 1) + bit;
        }

        bit >>= 2;
    }

    return res;
}
