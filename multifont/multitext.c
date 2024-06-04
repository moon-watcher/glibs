#include "multitext.h"

#include "config.h"

void multitext_init(multitext *const mt, multifont *const mf, unsigned int chars_number)
{
    mt->mf = mf;
    mt->chars_number = chars_number ?: mf->chars_number;
    mt->pos_in_tileset = mf->pal_counter * mf->chars_number;
    multitext_reset(mt);

    ++mf->pal_counter;
}

void multitext_reset(multitext *const mt)
{
    memset(mt->chars_vrampos, 0, mt->chars_number * sizeof(mt->chars_number));
}

void multitext_writeEx(multitext *const mt, char *text, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    char chr;
    const char *string = text;
    multifont *const mf = mt->mf;
    unsigned int const width = mf->char_width;
    unsigned char const height = mf->char_height;
    unsigned int const pos_in_tileset = mt->pos_in_tileset;
    const unsigned long *tiles_ptr = mf->tiles_ptr;
    unsigned int const chars_number = mt->chars_number;
    unsigned int const tiles = width * height;
    unsigned int (*vrampos_f)(unsigned int) = mf->vrampos_f;

    if (plan < 0) plan = mf->nb_plan;
    if (pal  < 0) pal  = mf->pal;
    if (prio < 0) prio = mf->prio;

    #include "config.h"
    #include MULTITEXT_LOAD_FILE
    #include MULTITEXT_WRITE_FILE

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= chars_number)
            continue;

        unsigned int *const vrampos = &mt->chars_vrampos[(unsigned int)chr];

        if (*vrampos == 0)
            MULTITEXT_LOAD_FUNCTION(*vrampos = vrampos_f(tiles), tiles_ptr, pos_in_tileset, chr, tiles);

        MULTITEXT_WRITE_FUNCTION(plan, pal, prio, *vrampos, x, y, width, height);
        x += width;
    }
}

void multitext_write(multitext *const mt, char *text, unsigned int x, unsigned int y)
{
    multitext_writeEx(mt, text, x, y, -1, -1, -1);
}