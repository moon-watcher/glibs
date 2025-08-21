#include "SGDK.h"

/**
 * Use compression & rescomp automatic timer
 */
int frameloader_SGDK_updater_0(frameloader *const this)
{
    SpriteDefinition *const sd = this->resource;
    Animation *const animation = sd->animations[this->anim];
    AnimationFrame *const frame = animation->frames[this->frame];

    /** Copy of loadTiles() in sprite_eng.c START */
    TileSet *const tileset = frame->tileset;
    u16 const lenInWord = tileset->numTile << 4;
    void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);
    u16 const vrampos = this->vrampos << 5;

    if (tileset->compression)
        unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2));
    else
        DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);
    /** Copy of loadTiles() in sprite_eng.c END */

    /**
     * Use rescomp to set .countdown in case
     * .timer = 0 as an automatic timer feature
     **/
    if (this->timer == 0)
        this->countdown = frame->timer;

    return animation->numFrame;
}

/**
 * W/O compresion & implement DMA_queueDma()
 */
int frameloader_SGDK_updater(frameloader *const this)
{
    SpriteDefinition *const sd = this->resource;
    Animation *const animation = sd->animations[this->anim];
    AnimationFrame *const frame = animation->frames[this->frame];
    TileSet *const tileset = frame->tileset;

    u16 len = tileset->numTile << 4;
    void *const from = FAR_SAFE(tileset->tiles, len << 1);
    u16 const to = this->vrampos << 5;

    /** Copy of DMA_queueDma() START */
    u32 const fromAddr = (u32)from;
    u32 const bankLimitB = 0x20000 - (fromAddr & 0x1FFFF);
    u32 const bankLimitW = bankLimitB >> 1;

    if (len > bankLimitW)
    {
        DMA_queueDmaFast(DMA_VRAM, (void *)(fromAddr + bankLimitB), to + bankLimitB, len - bankLimitW, 2);
        len = bankLimitW;
    }

    DMA_queueDmaFast(DMA_VRAM, from, to, len, 2);
    /** Copy of DMA_queueDma() END */

    return animation->numFrame;
}