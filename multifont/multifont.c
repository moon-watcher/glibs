#include "multifont.h"

void multifont_init(multifont *const mf, const Image *image, unsigned int plan, unsigned int pal, unsigned int (*vramPos_f)(unsigned int))
{
    mf->image = image;
    mf->pal = pal;
    mf->plan = plan;
    mf->prio = 1;
    mf->vramPos_f = vramPos_f;
}

void multifont_text_init(multifontText *const mft, multifont *const mf, unsigned int pos)
{
    mft->mf = mf;
    mft->pos_in_tileset = pos;
    mft->width = 1;
    mft->height = 1;
    memset(mft->chars_vrampos, 0, 128 * 2);
}

void multifont_text_setWidth(multifontText *const mft, unsigned int size)
{
    mft->width = size;
}

void multifont_text_setHeight(multifontText *const mft, unsigned int size)
{
    mft->height = size;
}

void multifont_text_writeCharEx(multifontText *const mft, char chr, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    multifont *const mf = mft->mf;
    u32 *const ptr_tiles = mf->image->tileset->tiles + (mft->pos_in_tileset << 3);
    unsigned int (*vramPos_f)(unsigned int) = mf->vramPos_f;
    unsigned int tiles = mft->width * mft->height;

    if (plan < 0) plan = mf->plan;
    if (pal  < 0) pal  = mf->pal;
    if (prio < 0) prio = mf->prio;

    chr -= 32;
    unsigned int *vrampos = &mft->chars_vrampos[(unsigned int)chr];

    if (!*vrampos)
    {
        *vrampos = vramPos_f(tiles);
        VDP_loadTileData(ptr_tiles + (chr << 3), *vrampos, tiles, CPU);
        // DMA_transfer(tm, DMA_VRAM, (void*) data, index * 32, num * 16, 2);
        // DMA_doCPUCopy(location, from, to, len, step);
        // DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)to), from, len, step);
        // DMA_doCPUCopyDirect(u32 cmd, void* from, unsigned int len, int step)
    }

    // cambiar por versión *Rect
    // void VDP_setTileMapDataRect  (VDPPlane plane, const unsigned int *data, unsigned int x,        unsigned int y, unsigned int w, unsigned int h, unsigned int wm, TransferMethod tm);
    // void VDP_setTileMapDataRectEx(VDPPlane plane, const unsigned int *data, unsigned int basetile, unsigned int x, unsigned int y, unsigned int w, unsigned int h,  unsigned int wm, TransferMethod tm);

    VDP_setTileMapXY(plan, TILE_ATTR_FULL(pal, prio, 0, 0, *vrampos), x, y);
}

void multifont_text_writeChar(multifontText *const mft, char chr, unsigned int x, unsigned int y)
{
    multifont_text_writeCharEx(mft, chr, x, y, -1, -1, -1);
}

void multifont_text_writeEx(multifontText *const mft, const char *const text, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    char chr;
    const char *string = text;

    while ((chr = *string++))
    {
        multifont_text_writeCharEx(mft, chr, x, y, plan, pal, prio);
        x += mft->width;
    }
}

void multifont_text_write(multifontText *const mft, const char *const text, unsigned int x, unsigned int y)
{
    multifont_text_writeEx(mft, text, x, y, -1, -1, -1);
}