#include "frameloader.h"

#include "config/update.h"
#include "config/setSprite.h"

#include GLIBS_FRAMELOADER_UPDATE_FILE
#include GLIBS_FRAMELOADER_SETSPRITE_FILE

void frameloader_init(frameloader *const fl, void **animations, unsigned int maxNumTile, unsigned int (*vrampos_f)(unsigned int))
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
    if (0 == fl->timer--)
        GLIBS_FRAMELOADER_UPDATE_FUNCTION(fl);
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    GLIBS_FRAMELOADER_SETSPRITE_FUNCTION(fl, sp);
}

void frameloader_setAnim(frameloader *const fl, unsigned int anim)
{
    fl->anim = anim;
}

void frameloader_reset(frameloader *const fl)
{
    fl->frame = 0;
    fl->timer = 0;
}