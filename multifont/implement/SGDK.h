#include <genesis.h>

#define _load(mf, vrampos, char, size) \
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), mf->tiles_ptr + ((mf->pos_in_tileset + chr) << 3), size << 4, 2)

#define _write(mf, vrampos, x, y, width, height) \
    VDP_fillTileMapRectInc(mf->plan, TILE_ATTR_FULL(mf->pal, mf->prio, 0, 0, vrampos), x, y, width, height)

#define _sprite(mfs, vrampos, x, y) \
    SPR_addSpriteEx(mfs->definition, x, y, TILE_ATTR_FULL(mfs->mf->pal, mfs->mf->prio, 0, 0, vrampos), mfs->flags)
