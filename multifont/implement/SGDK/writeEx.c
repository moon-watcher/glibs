#include <genesis.h>

void SGDK_writeEx(const unsigned long *tiles, unsigned int pos, char chr, unsigned int x, unsigned int y, int plan, int pal, int prio, unsigned char width, unsigned char height, unsigned int *const vrampos, int load)
{
    if (load)
        DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(*vrampos << 5)), (tiles + (pos << 3) + (chr << 3)), ((width * height) << 4), 2);
    
    VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(pal, prio, 0, 0, *vrampos), x, y, width, height);
}