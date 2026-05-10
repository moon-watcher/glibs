#pragma once

#include <genesis.h>
#include "../frameloader.h"

void frameloader_SGDK_sprite_updater(frameloader *$);

#define FRAMELOADER_SGDK_SPRITEDEF(D, X) \
    (D).animations[X], (D).animations[X]->numFrame
