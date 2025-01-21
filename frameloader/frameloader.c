#include "frameloader.h"
#include "config.h"

void frameloader_init(frameloader *const fl, void *const resource, unsigned vrampos)
{
    fl->resource = resource;
    fl->vrampos = vrampos;

    frameloader_setAnim(fl, 0);
    frameloader_update(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (fl->timer > 0)
    {
        fl->timer--;
        return;
    }

    FRAMELOADER_UPDATE(fl);
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    FRAMELOADER_SET_SPRITE(fl, sp);
}

void frameloader_setAnim(frameloader *const fl, unsigned anim)
{
    fl->anim = anim;
    fl->frame = 0;
    fl->timer = 0;
}