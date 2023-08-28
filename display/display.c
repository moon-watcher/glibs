#include <genesis.h>
#include "display.h"
#include "../multifont/multifont.h"

static u16 status;
static u16 cache[64];
static const u16 blacks[64] = {[0 ... 63] = 0x0000};

static void display(u16 frames, u16 *colors)
{
    SYS_doVBlankProcess();
    SYS_disableInts();

    if (frames)
        PAL_fadeTo(0, 63, (u16 *)colors, frames, 0);
    else
        PAL_setColors(0, (u16 *)colors, 64, DMA);

    SYS_enableInts();
}

////////////////////////////////////////////////////////////////////////////////

void display_init()
{
    status = 0;
    memcpy(cache, blacks, 64 * 2);
}

void display_on(u16 frames)
{
    if (status)
        return;

    status = !status;
    display(frames, cache);
}

void display_off(u16 frames)
{
    if (!status)
        return;

    display_init();
    display(frames, blacks);
}

void display_prepare(void *ptr, s16 pal, u16 type)
{
    u16 *colors = ptr;

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

    memcpy(cache + pal * 16, colors, 16 * 2);
}