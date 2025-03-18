#include "frameloader.h"
#include "config.h"

void frameloader_init(frameloader *const fl, void *const resource, unsigned vrampos, int timer)
{
    fl->vrampos = vrampos;
    frameloader_resume(fl);
    frameloader_resource(fl, resource, timer);
}

void frameloader_resource(frameloader *const fl, void *const resource, int timer)
{
    fl->resource = resource;
    frameloader_setAnim(fl, 0, timer);
    frameloader_update(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (fl->pause || (fl->countdown > 0 && --fl->countdown))
        return;

    fl->countdown = fl->timer;
    
    FRAMELOADER_UPDATE(fl);
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
