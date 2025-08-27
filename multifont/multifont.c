#include "multifont.h"

#define PROCCESS(CODE, MULTIPLY)                                           \
    unsigned short const width = mf->char_width;                           \
    unsigned short const height = mf->char_height;                         \
    unsigned short const nb_tiles = width * height;                        \
    unsigned short x = x_pos;                                              \
    unsigned short y = y_pos;                                              \
                                                                           \
    for (char chr, *string = text; (chr = *string++);)                     \
    {                                                                      \
        chr -= 32;                                                         \
                                                                           \
        if (chr >= mf->chars_number)                                       \
            continue;                                                      \
                                                                           \
        unsigned short *const vrampos = &mf->chars_vrampos[(unsigned)chr]; \
                                                                           \
        if (*vrampos == 0)                                                 \
        {                                                                  \
            *vrampos = mf->vrampos_f(nb_tiles);                            \
            _load(mf, *vrampos, chr, nb_tiles);                            \
        }                                                                  \
                                                                           \
        CODE;                                                              \
        x += width * MULTIPLY;                                             \
    }

//

void multifont_text_init(multifont *const mf, const unsigned long *tiles_ptr, unsigned plan, unsigned pal, unsigned (*vrampos_f)(unsigned), unsigned pos_in_tileset, unsigned chars_number)
{
    if (chars_number > MULTIFONT_MAXCHARS || chars_number == 0)
        chars_number = MULTIFONT_MAXCHARS;

    mf->tiles_ptr = tiles_ptr;
    mf->plan = plan;
    mf->pal = pal;
    mf->prio = 1;
    mf->vrampos_f = vrampos_f;
    mf->char_width = 1;
    mf->char_height = 1;
    mf->chars_number = chars_number;
    mf->pos_in_tileset = pos_in_tileset;

    multifont_text_reset(mf);
}

void multifont_text_write(multifont *const mf, const char *text, unsigned x_pos, unsigned y_pos)
{
    PROCCESS(
        _write(mf, *vrampos, x, y, width, height),
        1);
}

void multifont_text_reset(multifont *const mf)
{
    for (unsigned short i = 0; i < mf->chars_number; i++)
        mf->chars_vrampos[i] = 0;
}

//

void multifont_sprite_init(multifont_sprite *const mfs, multifont *const mf, void *const def, void(*freeFn))
{
    for (unsigned i = 0; i < MULTIFONT_SPRITE_MAXSPRITES; i++)
        mfs->array[i] = 0;

    mfs->total = 0;
    mfs->mf = mf;
    mfs->definition = def;
    mfs->freeFn = freeFn;
}

void multifont_sprite_write(multifont_sprite *const mfs, const char *text, unsigned x_pos, unsigned y_pos)
{
    multifont *const mf = mfs->mf;
    PROCCESS(
        mfs->array[mfs->total++] = _sprite(mf, *vrampos, x, y, mfs->definition),
        MULTIFONT_SPRITE_TILEWIDTH);
}

void multifont_sprite_reset(multifont_sprite *const mfs)
{
    if (mfs->freeFn)
        for (unsigned i = 0; i < MULTIFONT_SPRITE_MAXSPRITES; i++)
            if (mfs->array[i])
                mfs->freeFn(mfs->array[i]);
        
    for (unsigned i = 0; i < MULTIFONT_SPRITE_MAXSPRITES; i++)
        mfs->array[i] = 0;

    mfs->total = 0;
}
