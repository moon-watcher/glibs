#include "multifont.h"

void multifont_init(multifont *const mt, const unsigned long *tiles_ptr, unsigned plan, unsigned pal, unsigned (*vrampos_f)(unsigned), unsigned pos_in_tileset, unsigned chars_number)
{
    if (chars_number > MULTIFONT_MAXCHARS)
        chars_number = MULTIFONT_MAXCHARS;

    mt->tiles_ptr = tiles_ptr;
    mt->plan = plan;
    mt->pal = pal;
    mt->prio = 1;
    mt->vrampos_f = vrampos_f;
    mt->char_width = 1;
    mt->char_height = 1;
    mt->chars_number = chars_number ?: MULTIFONT_MAXCHARS;
    mt->pos_in_tileset = pos_in_tileset;
    
    multifont_reset(mt);
}

void multifont_write(multifont *const mt, char *text, unsigned x, unsigned y)
{
    char chr;
    char *string = text;
    unsigned width = mt->char_width;
    unsigned height = mt->char_height;
    unsigned nb_tiles = width * height;

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= mt->chars_number)
            continue;

        unsigned *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
            _load(*vrampos = mt->vrampos_f(nb_tiles), mt->tiles_ptr, mt->pos_in_tileset, chr, nb_tiles);

        _write(mt->plan, mt->pal, mt->prio, *vrampos, x, y, width, height);
        x += width;
    }
}

void multifont_reset(multifont *const mt)
{
    memset(mt->chars_vrampos, 0, mt->chars_number * sizeof(mt->chars_number));
}
