#pragma once

#include <genesis.h>

#define FRAMELOADER_ANIM(FL, ANIM)                                               \
    FL->anim = ANIM;                                                             \
    FL->frame = 0;                                                               \
    Animation *const animation = *(Animation **)FL->animations[FL->anim = ANIM]; \
    AnimationFrame *const frame = animation->frames[FL->frame];                  \
    FL->timer = frame->timer;

#define FRAMELOADER_UPDATE(FL)                                               \
    if (1 != FL->timer--)                                                    \
        return;                                                              \
                                                                             \
    Animation *const animation = *(Animation **)FL->animations[FL->anim];    \
    AnimationFrame *const frame = animation->frames[FL->frame];              \
    FL->timer = frame->timer;                                                \
                                                                             \
    if (++FL->frame == animation->numFrame)                                  \
        FL->frame = 0;                                                       \
                                                                             \
    /* Copy of loadTiles() in sprite_eng.c */                                \
    u8 *buf;                                                                 \
    TileSet *const ts = frame->tileset;                                      \
    u16 const compression = ts->compression;                                 \
    u16 const lenInWord = (ts->numTile << 5) >> 1;                           \
    void *const from = FAR_SAFE(ts->tiles, lenInWord << 1);                  \
    u16 const vrampos = FL->vram << 5;                                       \
                                                                             \
    if (compression == COMPRESSION_NONE)                                     \
        DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);                 \
    else if (buf = DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2)) \
        unpack(compression, (u8 *)from, buf);

#define FRAMELOADER_SPRITE(FL, SP)                  \
    SPR_setAnimAndFrame((Sprite *)SP, FL->anim, 0); \
    SPR_setVRAMTileIndex((Sprite *)SP, FL->vram);   \
    SPR_setAutoTileUpload((Sprite *)SP, 0);
