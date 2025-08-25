#include "multifont.h"

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

void multifont_write(multifont *const mt, const char *text, unsigned pos_x, unsigned pos_y)
{
    char chr;
    char *string = text;
    unsigned width = mt->char_width;
    unsigned height = mt->char_height;
    unsigned nb_tiles = width * height;
    unsigned x = pos_x;
    unsigned y = pos_y;

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= mt->chars_number)
            continue;

        unsigned *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
        {
            *vrampos = mt->vrampos_f(nb_tiles);
            _load(*vrampos, mt->tiles_ptr, mt->pos_in_tileset, chr, nb_tiles);
        }

        _write(mt, *vrampos, x, y, width, height);
        x += width;
    }
}

unsigned multifont_sprite(multifont *const mt, const char *text, unsigned pos_x, unsigned pos_y, void *const definition, void *array[])
{
    char chr;
    char *string = text;
    unsigned width = mt->char_width;
    unsigned height = mt->char_height;
    unsigned nb_tiles = width * height;
    unsigned x = pos_x;
    unsigned y = pos_y;
    unsigned total = 0;

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= mt->chars_number)
            continue;

        unsigned *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
        {
            *vrampos = mt->vrampos_f(nb_tiles);
            _load(*vrampos, mt->tiles_ptr, mt->pos_in_tileset, chr, nb_tiles);
        }

        array[total++] = _sprite(mt, *vrampos, x, y, definition);
        x += width * 8;
    }

    return total;
}

void multifont_reset(multifont *const mt)
{
    for (int i = 0; i < mt->chars_number; i++)
        mt->chars_vrampos[i] = 0;
}
