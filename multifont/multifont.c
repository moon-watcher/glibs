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


// void multifont_sample()
// {
//     multifont mf;
//     multitext mft1;
//     multitext mft2;

//     vraminc_init();

//     multifont_init(&mf, MULTIFONT_TILES(&resource_palette_font_main), MULTIFONT_PAL(&resource_palette_font_main), BG_B, PAL0, &vraminc_get, 128);
//     // multifont_setPalette(&mf);

//     multifont_init_pal(&mft1, &mf);
//     multifont_init_pal(&mft2, &mf);

//     multitext_write(&mft1, "ABA C", 10, 10);
//     multitext_write(&mft2, "QERXX", 20, 11);
// }