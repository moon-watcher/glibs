#pragma once

#include "SGDK.h"

void frameloader_SGDK_updater(frameloader *const this)
{
    Animation *const animation = ((SpriteDefinition *const)this->resource)->animations[this->anim];

    if (this->timer < 0)
        this->countdown = animation->frames[this->frame]->timer;

    /* Copy of loadTiles() in sprite_eng.c */
    TileSet *const tileset = animation->frames[this->frame]->tileset;
    u16 const lenInWord = (tileset->numTile << 5) >> 1;

    if (lenInWord)
    {
        void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);
        u16 const vrampos = this->vrampos << 5;

        if (tileset->compression)
            unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2));
        else
            DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);
    }

    if (++this->frame == animation->numFrame)
        this->frame = 0;
}

void frameloader_SGDK_setSprite(frameloader *const this, Sprite *const sprite)
{
    SPR_setDefinition(sprite, this->resource);
    SPR_setAnimAndFrame(sprite, this->anim, 0);
    SPR_setAutoTileUpload(sprite, 0);
    SPR_setVRAMTileIndex(sprite, this->vrampos);
}