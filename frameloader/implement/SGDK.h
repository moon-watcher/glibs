#pragma once

#include <genesis.h>

#define FRAMELOADER_UPDATE(FL)                                                                                                                                                           \
    Animation *const animation = ((SpriteDefinition *const) FL->definition)->animations[fl->anim];                                                                                           \
    fl->timer = animation->frames[fl->frame]->timer;                                                                                                                                     \
                                                                                                                                                                                         \
    /* Copy of loadTiles() in sprite_eng.c */                                                                                                                                            \
    TileSet *const tileset = animation->frames[fl->frame]->tileset;                                                                                                                      \
    u16 const lenInWord = (tileset->numTile << 5) >> 1;                                                                                                                                  \
                                                                                                                                                                                         \
    if (lenInWord)                                                                                                                                                                       \
    {                                                                                                                                                                                    \
        void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);                                                                                                                     \
        u16 const vrampos = fl->vrampos << 5;                                                                                                                                            \
        tileset->compression ? unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2)) : DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2); \
    }                                                                                                                                                                                    \
                                                                                                                                                                                         \
    if (++fl->frame == animation->numFrame)                                                                                                                                              \
    {                                                                                                                                                                                    \
        fl->frame = 0;                                                                                                                                                                   \
    }

#define FRAMELOADER_SET_SPRITE(FL, SP)               \
    SPR_setAnimAndFrame((Sprite *)SP, FL->anim, 0);  \
    SPR_setVRAMTileIndex((Sprite *)SP, FL->vrampos); \
    SPR_setAutoTileUpload((Sprite *)SP, 0);
