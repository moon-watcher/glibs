#include <genesis.h>

static void _load(unsigned int vrampos, const unsigned long *tiles, unsigned int pos, char chr, unsigned int size)
{
    DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(vrampos << 5)), tiles + ((pos + chr) << 3), size << 4, 2);
}

static void _write(int plan, int pal, int prio, unsigned int vrampos, unsigned int x, unsigned int y, unsigned char width, unsigned char height)
{
    VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(pal, prio, 0, 0, vrampos), x, y, width, height);
}