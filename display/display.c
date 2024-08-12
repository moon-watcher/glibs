#include "display.h"
#include "config.h"

static unsigned short cache[DISPLAY_PALETTE_COLORS * DISPLAY_PALETTES];

void display_on(unsigned char frames)
{
    DISPLAY_FADE_FUNCTION(frames, cache);
}

void display_off(unsigned char frames)
{
    memset(cache, 0, sizeof(cache));
    display_on(frames);
}

void display_prepare(unsigned short *colors, unsigned char pal)
{
    memcpy(cache + pal * DISPLAY_PALETTE_COLORS, colors, sizeof(cache) / DISPLAY_PALETTES);
}