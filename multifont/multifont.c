#include "multifont.h"

void multifont_init(multifont *const mf, const unsigned long *tiles_ptr, const unsigned int *palette_data, unsigned int plan, unsigned int pal, unsigned int (*vrampos_f)(unsigned int), unsigned int chars_number)
{
    if (chars_number > MULTITEXT_MAX_CHARS)
        chars_number = MULTITEXT_MAX_CHARS;

    mf->tiles_ptr = tiles_ptr;
    mf->palette_data = palette_data;
    mf->nb_plan = plan;
    mf->pal = pal;
    mf->prio = 1;
    mf->vrampos_f = vrampos_f;
    mf->chars_number = chars_number ?: MULTITEXT_MAX_CHARS;
    mf->pal_counter = 0;
    mf->char_width = 1;
    mf->char_height = 1;
}