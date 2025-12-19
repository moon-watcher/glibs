#include <genesis.h>

#define MULTIFONT_TILE_LOAD(mf, vrampos, chr, size) \
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), mf->tiles_ptr + ((mf->pos_in_tileset + chr) << 3), size << 4, 2)

#define MULTIFONT_TILE_WRITE(mf, vrampos, x, y, width, height) \
    VDP_fillTileMapRectInc(mf->plan, TILE_ATTR_FULL(mf->pal, mf->prio, 0, 0, vrampos), x, y, width, height)

#define MULTIFONT_SPRITE_LOAD(mf, vrampos, chr, size) \
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), mf->tiles_ptr + ((mf->pos_in_tileset + chr) << 3), size << 4, 2)

#define MULTIFONT_SPRITE_WRITE(mf, vrampos, x, y, mfs) \
    SPR_addSpriteEx(mfs->definition, x, y, TILE_ATTR_FULL(mf->pal, mf->prio, 0, 0, vrampos), mfs->flags)
