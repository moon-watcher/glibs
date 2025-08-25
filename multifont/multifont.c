#include "multifont.h"

#define TILE_WIDTH 8 // pixels

void multifont_init(multifont *const mt, const unsigned long *tiles_ptr, unsigned plan, unsigned pal, unsigned (*vrampos_f)(unsigned), unsigned pos_in_tileset, unsigned chars_number)
{
    if (chars_number > MULTIFONT_MAXCHARS || chars_number == 0)
        chars_number = MULTIFONT_MAXCHARS;

    mt->tiles_ptr = tiles_ptr;
    mt->plan = plan;
    mt->pal = pal;
    mt->prio = 1;
    mt->vrampos_f = vrampos_f;
    mt->char_width = 1;
    mt->char_height = 1;
    mt->chars_number = chars_number;
    mt->pos_in_tileset = pos_in_tileset;

    multifont_reset(mt);
}

unsigned multifont_text(multifont *const mt, const char *text, unsigned x, unsigned y)
{
    unsigned short const width = mt->char_width;
    unsigned short const height = mt->char_height;
    unsigned short const nb_tiles = width * height;
    unsigned short total = 0;

    for (char chr, *string = text; chr = *string++;)
    {
        chr -= 32;

        if (chr >= mt->chars_number)
            continue;

        unsigned short *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
        {
            *vrampos = mt->vrampos_f(nb_tiles);
            _load(mt, *vrampos, chr, nb_tiles);
        }

        _write(mt, *vrampos, x, y, width, height);
        x += width;
        ++total;
    }

    return total;
}

unsigned multifont_sprite(multifont *const mt, const char *text, unsigned x, unsigned y, void *const definition, void *array[])
{
    unsigned short const width = mt->char_width;
    unsigned short const height = mt->char_height;
    unsigned short const nb_tiles = width * height;
    unsigned short total = 0;

    for (char chr, *string = text; chr = *string++;)
    {
        chr -= 32;

        if (chr >= mt->chars_number)
            continue;

        unsigned short *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
        {
            *vrampos = mt->vrampos_f(nb_tiles);
            _load(mt, *vrampos, chr, nb_tiles);
        }

        array[total] = _sprite(mt, *vrampos, x, y, definition);
        x += width * TILE_WIDTH;
        ++total;
    }

    return total;
}

void multifont_reset(multifont *const mt)
{
    for (unsigned short i = 0; i < mt->chars_number; i++)
        mt->chars_vrampos[i] = 0;
}
