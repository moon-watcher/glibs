#include <genesis.h>

#include "../../../core/frameloader/frameloader.h"

static void SGDK_setSprite(frameloader *const fl, Sprite *const sp)
{
    SPR_setAnimAndFrame(sp, fl->anim, 0);
    SPR_setVRAMTileIndex(sp, fl->vram);
    SPR_setAutoTileUpload(sp, FALSE);
}