#include <genesis.h>

static void _update(frameloader *const fl)
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

static void _initTimer(frameloader *const fl)
{
    Animation *const animation = fl->animations[fl->anim];
    fl->timer = animation->frames[fl->frame]->timer;
}

static void _setSprite(frameloader *const fl, Sprite *const sp)
{
    SPR_setAnimAndFrame(sp, fl->anim, 0);
    SPR_setVRAMTileIndex(sp, fl->vram);
    SPR_setAutoTileUpload(sp, FALSE);
}