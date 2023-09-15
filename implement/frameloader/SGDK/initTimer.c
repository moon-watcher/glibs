#include <genesis.h>

#include "../../../core/frameloader/frameloader.h"

static void SGDK_initTimer(frameloader *const fl)
{
    Animation *const animation = fl->animations[fl->anim];
    fl->timer = animation->frames[fl->frame]->timer;
}