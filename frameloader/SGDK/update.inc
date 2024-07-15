Animation *const animation = fl->animations[fl->anim];
AnimationFrame *const frame = animation->frames[fl->frame];

fl->timer = frame->timer;

if (++fl->frame == animation->numFrame)
    fl->frame = 0;

// Copy of loadTiles() at sprite_eng.c
TileSet *const ts = frame->tileset;
u16 const compression = ts->compression;
u16 const lenInWord = (ts->numTile << 5) >> 1;
void *const from = FAR_SAFE(ts->tiles, lenInWord << 1);
u16 const vrampos = fl->vram << 5;

if (compression != COMPRESSION_NONE)
{
    u8 *buf = DMA_allocateAndQueueDma(DMA_VRAM, vrampos, lenInWord, 2);

    if (buf)
        unpack(compression, (u8 *)from, buf);
}
else
    DMA_queueDma(DMA_VRAM, from, vrampos, lenInWord, 2);
