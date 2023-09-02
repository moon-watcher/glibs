#include <genesis.h>
#include "display.h"
#include "../multifont/multifont.h"

static unsigned int status = 0;
static unsigned int cache[64];

void display_init()
{
    status = 0;
    memset(cache, 0, sizeof(cache));
}

void display_on(unsigned int frames)
{
    if (status)
        return;

    status = !status;
    display_fade(frames, cache);
}

void display_off(unsigned int frames)
{
    if (!status)
        return;

    display_init();
    display_fade(frames, 0);
}

void display_prepare(void *const ptr, int pal, unsigned int type)
{
    unsigned int *colors = ptr;

    if (type == DISPLAY_MULTIFONT)
    {
        colors = ((multifont *)ptr)->image->palette->data;
        pal = pal < 0 ? ((multifont *)ptr)->pal : pal;
    }

    else if (type == DISPLAY_SPRITEDEFINITION)
        colors = ((SpriteDefinition *)ptr)->palette->data;

    else if (type == DISPLAY_PALETTE)
        colors = ((Palette *)ptr)->data;

    else if (type == DISPLAY_SPRITE)
        colors = ((Sprite *)ptr)->definition->palette->data;

    else if (type == DISPLAY_IMAGE)
        colors = ((Image *)ptr)->palette->data;

    memcpy(cache + (pal << 4), colors, 32);
}

void display_fade(unsigned int frames, unsigned int *const colors)
{
    SYS_doVBlankProcess();
    SYS_disableInts();

    if (frames)
        PAL_fadeTo(0, 63, (unsigned int *)colors, frames, 0);
    else
        PAL_setColors(0, (unsigned int *)colors, 64, DMA);

    SYS_enableInts();
}