#include "frameloader.h"
#include "config.h"

void frameloader_init(frameloader *const fl, void **animations, unsigned vram)
{
    fl->animations = animations;
    fl->vram = vram;
    frameloader_anim(fl, 0);
    frameloader_reset(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (1 != fl->timer--)
        return;

    #include FRAMELOADER_UPDATE
}

void frameloader_sprite(frameloader *const fl, void *const sp)
{
    #include FRAMELOADER_SPRITE
}

void frameloader_anim(frameloader *const fl, unsigned anim)
{
    fl->anim = anim;
    #include FRAMELOADER_ANIM
}

void frameloader_reset(frameloader *const fl)
{
    fl->frame = 0;
    #include FRAMELOADER_RESET
}
