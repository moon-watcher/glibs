#include "frameloader.h"
#include "config.h"

void frameloader_init(frameloader *const fl, void **animations, unsigned vram)
{
    fl->timer = fl->anim = fl->frame = 0;
    fl->animations = animations;    
    fl->vram = vram;

    FRAMELOADER_ANIM(fl, 0);
}

void frameloader_update(frameloader *const fl)
{
    FRAMELOADER_UPDATE(fl);
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    FRAMELOADER_SPRITE(fl, sp);
}

void frameloader_setAnim(frameloader *const fl, unsigned anim)
{
    FRAMELOADER_ANIM(fl, anim);
}