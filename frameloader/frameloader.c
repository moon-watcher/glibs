#include "frameloader.h"
#include "config.h"

/**
 * TODO: Add thrid param to frameloader_init() to use SPR_loadAllFrames()
 */

void frameloader_init(frameloader *const fl, void *const resource, unsigned vrampos)
{
    fl->vrampos = vrampos;
    frameloader_resume(fl);
    frameloader_resource(fl, resource);
}

void frameloader_resource(frameloader *const fl, void *const resource)
{
    fl->resource = resource;
    frameloader_setAnim(fl, 0);
    frameloader_update(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (fl->pause)
        return;

    if (fl->timer > 0 && --fl->timer)
        return;

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

void frameloader_pause(frameloader *const fl)
{
    fl->pause = 1;
}

void frameloader_resume(frameloader *const fl)
{
    fl->pause = 0;
}
