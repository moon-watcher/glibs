#include "frameloader.h"

#include FRAMELOADER_UPDATE_FILE
#include FRAMELOADER_SETSPRITE_FILE
#include FRAMELOADER_INITTIMER_FILE 

void frameloader_init(frameloader *const fl, void **animations, unsigned maxNumTile, unsigned (*vrampos_f)(unsigned))
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
    if (1 == fl->timer--)
        FRAMELOADER_UPDATE_FUNCTION(fl);
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    FRAMELOADER_SETSPRITE_FUNCTION(fl, sp);
}

void frameloader_setAnim(frameloader *const fl, unsigned anim)
{
    fl->anim = anim;
}

void frameloader_reset(frameloader *const fl)
{
    fl->frame = 0;
    FRAMELOADER_INITTIMER_FUNCTION(fl);
}