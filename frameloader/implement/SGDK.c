#include "SGDK.h"

int frameloader_SGDK_updater(frameloader *const this)
{
    SpriteDefinition *const sd = this->resource;
    Animation *const animation = sd->animations[this->anim];
    AnimationFrame *const frame = animation->frames[this->frame];
    TileSet *const tileset = frame->tileset; // Copy of loadTiles() in sprite_eng.c
    u16 const lenInWord = tileset->numTile << 4;
    void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);
    u16 const vrampos = this->vrampos << 5;

    if (tileset->compression)
        unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2));
    else
        DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);

    if (this->timer == 0)
        this->countdown = frame->timer;

    return animation->numFrame;
}
