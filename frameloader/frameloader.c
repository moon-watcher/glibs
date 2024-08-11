#include "frameloader.h"
#include "implement/SGDK.h"

void frameloader_init(frameloader *const fl, void **animations, unsigned vram)
{
    fl->anim = fl->frame = 0;
    fl->animations = animations;
    fl->vram = vram;

    FRAMELOADER_ANIM
}

void frameloader_update(frameloader *const fl)
{
    if (1 != fl->timer--)
        return;

    FRAMELOADER_UPDATE
}

void frameloader_setSprite(frameloader *const fl, void *const sp)
{
    FRAMELOADER_SPRITE
}

void frameloader_setAnim(frameloader *const fl, unsigned anim)
{
    fl->frame = 0;
    fl->anim = anim;

    FRAMELOADER_ANIM
}