#include <genesis.h>

#include "../frameloader.h"

static void SGDK_initTimer(frameloader *const fl)
{
    Animation *const animation = fl->animations[fl->anim];
    fl->timer = animation->frames[fl->frame]->timer;
}