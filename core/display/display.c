#include "../../config/display.h"

#include "../../config/memcpy.h"
#include "../../config/memset.h"

static unsigned short cache[GLIBS_DISPLAY_PALETTE_COLORS * GLIBS_DISPLAY_PALETTES];

void display_on(unsigned char frames)
{
    #include GLIBS_DISPLAY_FADE_FILE

    GLIBS_DISPLAY_FADE_FUNCTION(frames, cache);
}

void display_off(unsigned char frames)
{
    memset(cache, 0, sizeof(cache));
    display_on(frames);
}

void display_prepare(unsigned short *colors, unsigned char pal)
{
    memcpy(cache + pal * GLIBS_DISPLAY_PALETTE_COLORS, colors, sizeof(cache) / GLIBS_DISPLAY_PALETTES);
}