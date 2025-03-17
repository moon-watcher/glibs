#include "frameloader.h"
#include "config.h"

/**
 * TODO: Add thrid param to frameloader_init() to use SPR_loadAllFrames()
 */

void frameloader_init(frameloader *const fl, void *const resource, unsigned vrampos, unsigned allFrames)
{
    fl->vrampos = vrampos;
    // fl->allFrames = allFrames;
    fl->staticTimer = -1;
    frameloader_resume(fl);
    frameloader_resource(fl, resource);
}

void frameloader_resource(frameloader *const fl, void *const resource)
{
    fl->resource = resource;
    frameloader_setAnim(fl, 0, fl->staticTimer);

    // if (fl->allFrames)
    //     SPR_loadAllFrames(fl->resource, fl->vrampos, fl->allFrames);

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

void frameloader_setSprite(frameloader *const fl, void *const sp, int staticTimer)
{
    FRAMELOADER_SET_SPRITE(fl, sp, staticTimer);
}

void frameloader_setAnim(frameloader *const fl, unsigned anim, int staticTimer)
{
    fl->anim = anim;
    fl->frame = 0;
    fl->timer = 0;
    fl->staticTimer = staticTimer;
}

void frameloader_pause(frameloader *const fl)
{
    fl->pause = 1;
}

void frameloader_resume(frameloader *const fl)
{
    fl->pause = 0;
}
