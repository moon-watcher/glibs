#include "SGDK.h"

/**
 * W/O compresion & implement DMA_queueDma()
 */
void frameloader_SGDK_updater(frameloader *$)
{
    TileSet *tileset = ((Animation *)$->resource)->frames[$->frame]->tileset;

    u16 len = tileset->numTile << 4;
    void *from = FAR_SAFE(tileset->tiles, len << 1);
    u16 to = $->vrampos << 5;

    /** Copy of DMA_queueDma() */
    u32 fromAddr = (u32)from;
    u32 bankLimitB = 0x20000 - (fromAddr & 0x1FFFF);
    u32 bankLimitW = bankLimitB >> 1;

    if (len > bankLimitW)
    {
        DMA_queueDmaFast(DMA_VRAM, (void *)(fromAddr + bankLimitB), to + bankLimitB, len - bankLimitW, 2);
        len = bankLimitW;
    }

    DMA_queueDmaFast(DMA_VRAM, from, to, len, 2);
}
