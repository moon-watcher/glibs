#include <genesis.h>

static inline void _load(u16 vrampos, u32 *tiles, u16 pos, u16 chr, u16 size)
{
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), tiles + ((pos + chr) << 3), size << 4, 2);
}

#define _write(mt, vrampos, x, y, width, height) \
    VDP_fillTileMapRectInc(mt->plan, TILE_ATTR_FULL(mt->pal, mt->prio, 0, 0, vrampos), x, y, width, height)

#define _sprite(mt, vrampos, x, y, definition) \
    SPR_addSpriteEx(definition, x, y, TILE_ATTR_FULL(mt->pal, mt->prio, 0, 0, vrampos), 0)
