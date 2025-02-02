#pragma once

#include <genesis.h>

#define FRAMELOADER_UPDATE(FL)                                                                                  \
    Animation *const animation = ((SpriteDefinition *const)FL->resource)->animations[FL->anim];                 \
    FL->timer = animation->frames[FL->frame]->timer;                                                            \
                                                                                                                \
    /* Copy of loadTiles() in sprite_eng.c */                                                                   \
    TileSet *const tileset = animation->frames[FL->frame]->tileset;                                             \
    u16 const lenInWord = (tileset->numTile << 5) >> 1;                                                         \
                                                                                                                \
    if (lenInWord)                                                                                              \
    {                                                                                                           \
        void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);                                            \
        u16 const vrampos = FL->vrampos << 5;                                                                   \
                                                                                                                \
        if (tileset->compression)                                                                               \
            unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2)); \
        else                                                                                                    \
            DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);                                                \
    }                                                                                                           \
                                                                                                                \
    if (++FL->frame == animation->numFrame)                                                                     \
        FL->frame = 0;

#define FRAMELOADER_SET_SPRITE(FL, SP)               \
    SPR_setAnimAndFrame((Sprite *)SP, FL->anim, 0);  \
    SPR_setVRAMTileIndex((Sprite *)SP, FL->vrampos); \
    SPR_setAutoTileUpload((Sprite *)SP, 0);
