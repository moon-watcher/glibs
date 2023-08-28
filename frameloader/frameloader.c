#include "frameloader.h"

void frameloader_init(frameloader *const fl, const SpriteDefinition *sd, unsigned int (*vrampos_f)(unsigned int))
{
    if (fl->animations != sd->animations)
    {
        fl->animations = sd->animations;
        fl->vram = vrampos_f(sd->maxNumTile);
    }

    frameloader_setAnim(fl, 0);
    frameloader_reset(fl);
}

void frameloader_update(frameloader *const fl)
{
    if (fl->timer--)
        return;

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

void frameloader_setSprite(frameloader *const fl, Sprite *const sp)
{
    SPR_setAnimAndFrame(sp, fl->anim, 0);
    SPR_setVRAMTileIndex(sp, fl->vram);
    SPR_setAutoTileUpload(sp, FALSE);
}

void frameloader_setAnim(frameloader *const fl, unsigned int anim)
{
    fl->anim = anim;
}

void frameloader_reset(frameloader *const fl)
{
    fl->frame = 0;
    fl->timer = 0;
}