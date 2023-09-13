#include "multitext.h"

#include "../_config/memset.h"
#include "config/multitext.h"

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
    memset(mt->chars_vrampos, 0, mt->mf->chars_number*sizeof(unsigned int));
}

void multitext_writeEx(multitext *const mt, const char *const text, unsigned int x, unsigned int y, int plan, int pal, int prio)
{
    char chr;
    const char *string = text;
    multifont *const mf = mt->mf;
    unsigned int const width = mf->char_width;
    unsigned char const height = mf->char_height;
    unsigned int const pos_in_tileset = mt->pos_in_tileset;
    const unsigned long *tiles_ptr = mf->tiles_ptr;
    unsigned int chars_number = mt->chars_number;
    unsigned int tiles = width * height;

    if (plan < 0) plan = mf->nb_plan;
    if (pal  < 0) pal  = mf->pal;
    if (prio < 0) prio = mf->prio;

    #include "config/writeEx.h"
    #include GLIBS_MULTITEXT_WRITE_FILE

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= chars_number)
            continue;

        unsigned int *const vrampos = &mt->chars_vrampos[(unsigned int)chr];
        int load = !*vrampos;

        if (*vrampos == 0)
        {
            *vrampos = mf->vrampos_f(tiles);
        }

        GLIBS_MULTITEXT_WRITE_FUNCTION(tiles_ptr, pos_in_tileset, chr, x, y, plan, pal, prio, width, height, vrampos, load);
        x += width;
    }
}

void multitext_write(multitext *const mt, const char *const text, unsigned int x, unsigned int y)
{
    multitext_writeEx(mt, text, x, y, -1, -1, -1);
}