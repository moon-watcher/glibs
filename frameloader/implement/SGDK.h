#pragma once

#include <genesis.h>
#include "../frameloader.h"

void frameloader_SGDK_updater(frameloader *const);

#define FRAMELOADER_SGDK_SPRITEDEF(D) \
    D.animations[0], D.animations[0]->numFrame

#define FRAMELOADER_SGDK_SPRITEDEF_EX(D, X) \
    D.animations[X], D.animations[X]->numFrame
