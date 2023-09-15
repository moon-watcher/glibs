#include <genesis.h>

#include "../../../core/frameloader/frameloader.h"

static void SGDK_update(frameloader *const fl)
{
    Animation *const animation = fl->animations[fl->anim];
    AnimationFrame *const frame = animation->frames[fl->frame];

    fl->timer = frame->timer;
    
    if (++fl->frame == animation->numFrame)
        fl->frame = 0;

    // Copy of loadTiles() at sprite_eng.c
    TileSet *const ts = frame->tileset;
    u16 compression = ts->compression;
    u16 lenInWord = (ts->numTile << 5) >> 1;
    void *const from = FAR_SAFE(ts->tiles, lenInWord << 1);
    unsigned int vrampos = fl->vram << 5;

    if (compression != COMPRESSION_NONE)
    {
        u8 *buf = DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2);

        if (buf)
            unpack(compression, (u8 *)from, buf);
    }
    else
        DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);
}