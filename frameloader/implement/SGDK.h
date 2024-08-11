#pragma once

#include <genesis.h>

#define FRAMELOADER_ANIM                                                  \
    Animation *const animation = *(Animation **)fl->animations[fl->anim]; \
    AnimationFrame *const frame = animation->frames[fl->frame];           \
    fl->timer = frame->timer;

#define FRAMELOADER_UPDATE                                                   \
    FRAMELOADER_ANIM                                                         \
                                                                             \
    if (++fl->frame == animation->numFrame)                                  \
        fl->frame = 0;                                                       \
                                                                             \
    /* Copy of loadTiles() at sprite_eng.c */                                \
    u8 *buf;                                                                 \
    TileSet *const ts = frame->tileset;                                      \
    u16 const compression = ts->compression;                                 \
    u16 const lenInWord = (ts->numTile << 5) >> 1;                           \
    void *const from = FAR_SAFE(ts->tiles, lenInWord << 1);                  \
    u16 const vrampos = fl->vram << 5;                                       \
                                                                             \
    if (compression == COMPRESSION_NONE)                                     \
        DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);                 \
    else if (buf = DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2)) \
        unpack(compression, (u8 *)from, buf);

#define FRAMELOADER_SPRITE                          \
    SPR_setAnimAndFrame((Sprite *)sp, fl->anim, 0); \
    SPR_setVRAMTileIndex((Sprite *)sp, fl->vram);   \
    SPR_setAutoTileUpload((Sprite *)sp, FALSE);
