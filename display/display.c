#include "../_config/memcpy.h"
#include "../_config/memset.h"

static unsigned int cache[64];

void display_on(unsigned int frames)
{
    #include "config/fade.h";
    #include FILE

    FADE(frames, cache);
}

void display_off(unsigned int frames)
{
    memset(cache, 0, 128);
    display_on(frames);
}

void display_prepare(unsigned int *colors, unsigned int pal)
{
    memcpy(cache + (pal << 3), colors, 32);
}