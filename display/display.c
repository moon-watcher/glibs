#include "config.h"
#include <stdint.h>

static uint16_t cache[DISPLAY_PALETTE_COLORS * DISPLAY_PALETTES];

void display_on(uint16_t frames)
{
    DISPLAY_FADE_FUNCTION(frames, cache);
}

void display_off(uint16_t frames)
{
    memset(cache, 0, sizeof(cache));
    display_on(frames);
}

uint16_t display_prepare(uint16_t *colors, uint16_t pal)
{
    memcpy(cache + pal * DISPLAY_PALETTE_COLORS, colors, sizeof(cache) / DISPLAY_PALETTES);

    return pal;
}

uint16_t display_prepareColor(uint16_t color, uint16_t index)
{
    return cache[index] = color;
}
