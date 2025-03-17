#include "frameloader.h"
#include "config.h"

/**
 * TODO: Add thrid param to frameloader_init() to use SPR_loadAllFrames()
 */

void frameloader_init(frameloader *const fl, void *const resource, unsigned vrampos, unsigned allTiles)
{
    fl->vrampos = vrampos;
    // fl->allTiles = allTiles;
    fl->timer = 0;
    frameloader_resume(fl);
    frameloader_resource(fl, resource);
}

void frameloader_resource(frameloader *const fl, void *const resource)
{
    fl->resource = resource;
    frameloader_setAnim(fl, 0, fl->timer);

    // if (fl->allTiles)
    //     SPR_loadAllFrames(fl->resource, fl->vrampos, fl->allFrames);

    frameloader_update(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (fl->pause)
        return;

    if (fl->countdown > 0 && --fl->countdown)
        return;

    fl->countdown = fl->timer;
    FRAMELOADER_UPDATE(fl);
}

void frameloader_setSprite(frameloader *const fl, void *const sp, int timer)
{
    fl->countdown = fl->timer;
    FRAMELOADER_SET_SPRITE(fl, sp, timer);
}

void frameloader_setAnim(frameloader *const fl, unsigned anim, int timer)
{
    fl->anim = anim;
    fl->frame = 0;
    fl->countdown = 0;
    fl->timer = timer;
}

void frameloader_pause(frameloader *const fl)
{
    fl->pause = 1;
}

void frameloader_resume(frameloader *const fl)
{
    fl->pause = 0;
}
