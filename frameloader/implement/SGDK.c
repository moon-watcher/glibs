#include "SGDK.h"

/**
 * W/O compresion & implement DMA_queueDma()
 */
void frameloader_SGDK_updater(frameloader *$)
{
    TileSet *tileset = ((Animation *)$->resource)->frames[$->frame]->tileset;

    u16 len = tileset->numTile << 4;
    u32 from = (u32)FAR_SAFE(tileset->tiles, len << 1);
    u32 to = $->vrampos << 5;

    /** Copy-esque of DMA_queueDma() */
    u32 bankLimitW = (0x20000 - (from & 0x1FFFF)) >> 1;

    if (len > bankLimitW)
    {
        u32 bankLimitB = bankLimitW << 1;
        DMA_queueDmaFast(DMA_VRAM, (void *)(from + bankLimitB), to + bankLimitB, len - bankLimitW, 2);
        len = bankLimitW;
    }

    DMA_queueDmaFast(DMA_VRAM, (void *)from, to, len, 2);
}
