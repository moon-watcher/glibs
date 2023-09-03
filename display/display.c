#include "config/display.h"

#include "../_config/memcpy.h"
#include "../_config/memset.h"

static char status = -1;
static unsigned short cache[GLIBS_DISPLAY_PALETTE_COLORS * GLIBS_DISPLAY_PALETTES];

void display_init()
{
    status = -1;
}

void display_on(unsigned char frames)
{
    if (status == 1)
        return;

    #include "config/fade.h"
    #include GLIBS_DISPLAY_FADE_FILE

    GLIBS_DISPLAY_FADE_FUNCTION(frames, cache);
    status = 1;     
}

void display_off(unsigned char frames)
{
    if (status == 0)
        return;

    memset(cache, 0, sizeof(cache));
    display_on(frames);
    status = 0;
}

void display_prepare(unsigned short *colors, unsigned char pal)
{
    memcpy(cache + pal * GLIBS_DISPLAY_PALETTE_COLORS, colors, sizeof(cache) / GLIBS_DISPLAY_PALETTES);
}