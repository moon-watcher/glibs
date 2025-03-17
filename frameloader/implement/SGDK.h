#pragma once

#include <genesis.h>

#define FRAMELOADER_UPDATE(fl)                                                                                  \
    Animation *const animation = ((SpriteDefinition *const)fl->resource)->animations[fl->anim];                 \
    fl->timer = (fl->staticTimer > 0) ? fl->staticTimer : animation->frames[fl->frame]->timer;                  \
                                                                                                                \
    /* Copy of loadTiles() in sprite_eng.c */                                                                   \
    TileSet *const tileset = animation->frames[fl->frame]->tileset;                                             \
    u16 const lenInWord = (tileset->numTile << 5) >> 1;                                                         \
                                                                                                                \
    if (lenInWord)                                                                                              \
    {                                                                                                           \
        void *const from = FAR_SAFE(tileset->tiles, lenInWord << 1);                                            \
        u16 const vrampos = fl->vrampos << 5;                                                                   \
                                                                                                                \
        if (tileset->compression)                                                                               \
            unpack(tileset->compression, (u8 *)from, DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2)); \
        else                                                                                                    \
            DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);                                                \
    }                                                                                                           \
                                                                                                                \
    if (++fl->frame == animation->numFrame)                                                                     \
        fl->frame = 0;

#define FRAMELOADER_SET_SPRITE(fl, sp, staticTimer)  \
    fl->staticTimer = staticTimer;                   \
    SPR_setAnimAndFrame((Sprite *)sp, fl->anim, 0);  \
    SPR_setVRAMTileIndex((Sprite *)sp, fl->vrampos); \
    SPR_setAutoTileUpload((Sprite *)sp, 0);
