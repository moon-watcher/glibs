#include <genesis.h>

#include "multitext.h"
#include "config.h"

void multitext_init(multitext *const mt, multifont *const mf)
{
    mt->mf = mf;
    mt->pos_in_tileset = mf->pal_counter * mf->chars_number;
    multitext_reset(mt);

    ++mf->pal_counter;
}

void multitext_reset(multitext *const mt)
{
    memset(mt->chars_vrampos, 0, mt->mf->chars_number);
}

void multitext_charEx(multitext *const mt, char chr, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    chr -= 32;

    multifont *const mf = mt->mf;

    if (chr >= mf->chars_number)
        return;

    unsigned char const width = mf->char_width;
    unsigned char const height = mf->char_height;
    unsigned int *const vrampos = &mt->chars_vrampos[(unsigned int)chr];

    if (!*vrampos)
    {
        unsigned int const tiles = width * height;
        *vrampos = mf->vrampos_f(tiles);

        DMA_doCPUCopyDirect(VDP_WRITE_VRAM_ADDR((u32)(*vrampos << 5)), (mf->tiles_ptr + (mt->pos_in_tileset << 3) + (chr << 3)), (tiles << 4), 2);
    }

    if (plan < 0) plan = mf->nb_plan;
    if (pal  < 0) pal  = mf->pal;
    if (prio < 0) prio = mf->prio;

    VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(pal, prio, 0, 0, *vrampos), x, y, width, height);
}

void multitext_char(multitext *const mt, char chr, unsigned int x, unsigned int y)
{
    multitext_charEx(mt, chr, x, y, -1, -1, -1);
}

void multitext_writeEx(multitext *const mt, const char *const text, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    char chr;
    const char *string = text;
    unsigned int const width = mt->mf->char_width;

    while ((chr = *string++))
    {
        multitext_charEx(mt, chr, x, y, plan, pal, prio);
        x += width;
    }
}

void multitext_write(multitext *const mt, const char *const text, unsigned int x, unsigned int y)
{
    multitext_writeEx(mt, text, x, y, -1, -1, -1);
}