#pragma once

#include <genesis.h>

#define FRAMELOADER_UPDATE(fl)                                                                                  \
    Animation *const animation = ((SpriteDefinition *const)fl->resource)->animations[fl->anim];                 \
                                                                                                                \
    if (fl->timer < 0)                                                                                          \
        fl->countdown = animation->frames[fl->frame]->timer;                                                    \
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

#define FRAMELOADER_SET_SPRITE(fl, sp)         \
    Sprite *const sprite = sp;                 \
    SPR_setDefinition(sprite, fl->resource);   \
    SPR_setAnimAndFrame(sprite, fl->anim, 0);  \
    SPR_setVRAMTileIndex(sprite, fl->vrampos); \
    SPR_setAutoTileUpload(sprite, 0);
