
//
// https://stackoverflow.com/questions/3581528/how-is-the-square-root-function-implemented/64039326#64039326
//
unsigned long sqrt(unsigned long x)
{
    unsigned long i = 0;
    if (x >= 65536)
    {
        if ((i + 32768) * (i + 32768) <= x)
            i += 32768;
        if ((i + 16384) * (i + 16384) <= x)
            i += 16384;
        if ((i + 8192) * (i + 8192) <= x)
            i += 8192;
        if ((i + 4096) * (i + 4096) <= x)
            i += 4096;
        if ((i + 2048) * (i + 2048) <= x)
            i += 2048;
        if ((i + 1024) * (i + 1024) <= x)
            i += 1024;
        if ((i + 512) * (i + 512) <= x)
            i += 512;
        if ((i + 256) * (i + 256) <= x)
            i += 256;
    }
    if ((i + 128) * (i + 128) <= x)
        i += 128;
    if ((i + 64) * (i + 64) <= x)
        i += 64;
    if ((i + 32) * (i + 32) <= x)
        i += 32;
    if ((i + 16) * (i + 16) <= x)
        i += 16;
    if ((i + 8) * (i + 8) <= x)
        i += 8;
    if ((i + 4) * (i + 4) <= x)
        i += 4;
    if ((i + 2) * (i + 2) <= x)
        i += 2;
    if ((i + 1) * (i + 1) <= x)
        i += 1;
    return i;
}

// ChatGPT alternative // not tested
unsigned long sqrt_fast(unsigned long x)
{
    unsigned long i = 0;
    unsigned long sq = 0;
    unsigned long step = (x >= 65536) ? 32768 : 128;

    do
    {
        unsigned long const newsq = sq + ((i << 1) + step) * step;
        if (newsq <= x)
        {
            i += step;
            sq = newsq;
        }
        step >>= 1;
    } while (step);

    return i;
}

#include <stdint.h>

// ChatGPT alternative // not tested
unsigned long sqrt_68k(unsigned long x)
{
    unsigned long res = 0;
    unsigned long bit = 1UL << 30; // mayor potencia de 4 <= x

    while (bit > x)
        bit >>= 2;

    while (bit)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }

    return res;
}


static inline uint16_t sqrt16(uint16_t x)
{
    uint16_t res = 0;
    uint16_t bit = 1 << 14; // mayor potencia de 4 <= 0xFFFF

    // ajustar bit inicial
    while (bit > x)
        bit >>= 2;

    while (bit)
    {
        uint16_t temp = res + bit;

        if (x >= temp)
        {
            x -= temp;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;
}

static inline uint16_t sqrt16_fast(uint16_t x)
{
    uint16_t res = 0;
    uint16_t bit = 1 << 14;

    while (bit)
    {
        uint16_t temp = res + bit;

        if (x >= temp)
        {
            x -= temp;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;
}


static inline uint16_t sqrt16_hybrid(uint16_t x)
{
    uint16_t res = 0;
    uint16_t bit;

    // Selección inicial SIN BUCLE (muy barato en 68K)
    if (x >= 16384)      bit = 1 << 14;
    else if (x >= 4096)  bit = 1 << 12;
    else if (x >= 1024)  bit = 1 << 10;
    else if (x >= 256)   bit = 1 << 8;
    else if (x >= 64)    bit = 1 << 6;
    else if (x >= 16)    bit = 1 << 4;
    else if (x >= 4)     bit = 1 << 2;
    else                 bit = 1 << 0;

    // núcleo estándar (igual que antes)
    while (bit)
    {
        uint16_t temp = res + bit;

        if (x >= temp)
        {
            x -= temp;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;
}

static inline uint16_t sqrt16_fastest(uint16_t x)
{
    const uint8_t sqrt_lut[256] = {
        0,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
        4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
        10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
        11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
        12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
        13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
        14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
        16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
        17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
        18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
        19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19
    };

    if (x == 0)
        return 0;

    // 1) aproximación ultra barata
    uint16_t r = sqrt_lut[x >> 8];

    // 2) refinamiento único (Newton 1 paso)
    r = (r + (x / r)) >> 1;

    return r;
}