#pragma once

#include <genesis.h>
#include "../frameloader.h"

void frameloader_SGDK_updater(frameloader *const);

#define FRAMELOADER_SGDK_SPRITEDEF(D, X) \
    D.animations[X], D.animations[X]->numFrame
