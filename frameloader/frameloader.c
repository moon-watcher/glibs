#include "frameloader.h"
#include "config.h"

void frameloader_init(frameloader *const fl, void **animations, unsigned maxNumTile, unsigned (*vrampos_f)())
{
    if (fl->animations != animations)
    {
        fl->animations = animations;
        fl->vram = vrampos_f(maxNumTile);
    }

    frameloader_setAnim(fl, 0);
    frameloader_reset(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (1 != fl->timer--)
        return;

    #include FRAMELOADER_UPDATE
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    #include FRAMELOADER_SPRITE
}

void frameloader_setAnim(frameloader *const fl, unsigned anim)
{
    fl->anim = anim;
}

void frameloader_reset(frameloader *const fl)
{
    fl->frame = 0;
    #include FRAMELOADER_RESET
}