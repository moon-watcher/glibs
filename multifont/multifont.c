#include "multifont.h"
#include MULTIFONT_IMPLEMENTATION

void multifont_init(multifont *const mf, const unsigned long *tiles_ptr, const unsigned *palette_data, unsigned plan, unsigned pal, unsigned (*vrampos_f)(unsigned), unsigned chars_number)
{
    if (chars_number > MULTIFONT_MAXCHARS)
        chars_number = MULTIFONT_MAXCHARS;

    mf->tiles_ptr = tiles_ptr;
    mf->palette_data = palette_data;
    mf->nb_plan = plan;
    mf->pal = pal;
    mf->prio = 1;
    mf->vrampos_f = vrampos_f;
    mf->chars_number = chars_number ?: MULTIFONT_MAXCHARS;
    mf->pal_counter = 0;
    mf->char_width = 1;
    mf->char_height = 1;
}

void multifont_text_prepare(multifont_text *const mt, multifont *const mf, unsigned chars_number)
{
    mt->mf = mf;
    mt->chars_number = chars_number ?: mf->chars_number;
    mt->pos_in_tileset = mf->pal_counter * mf->chars_number;
    multifont_text_reset(mt);

    ++mf->pal_counter;
}

void multifont_text_reset(multifont_text *const mt)
{
    memset(mt->chars_vrampos, 0, mt->chars_number * sizeof(mt->chars_number));
}

void multifont_text_writeEx(multifont_text *const mt, char *text, unsigned x, unsigned y, int plan, int pal, int prio)
{
    char chr;
    const char *string = text;
    multifont *const mf = mt->mf;
    unsigned const width = mf->char_width;
    unsigned char const height = mf->char_height;
    unsigned const pos_in_tileset = mt->pos_in_tileset;
    const unsigned long *tiles_ptr = mf->tiles_ptr;
    unsigned const chars_number = mt->chars_number;
    unsigned const tiles = width * height;
    unsigned (*vrampos_f)(unsigned) = mf->vrampos_f;

    if (plan < 0) plan = mf->nb_plan;
    if (pal  < 0) pal  = mf->pal;
    if (prio < 0) prio = mf->prio;

    while ((chr = *string++))
    {
        chr -= 32;

        if (chr >= chars_number)
            continue;

        unsigned *const vrampos = &mt->chars_vrampos[(unsigned)chr];

        if (*vrampos == 0)
            _load(*vrampos = vrampos_f(tiles), tiles_ptr, pos_in_tileset, chr, tiles);

        _write(plan, pal, prio, *vrampos, x, y, width, height);
        x += width;
    }
}

void multifont_text_write(multifont_text *const mt, char *text, unsigned x, unsigned y)
{
    multifont_text_writeEx(mt, text, x, y, -1, -1, -1);
}


// void multifont_sample()
// {
//     multifont mf;
//     multifont_text mft1;
//     multifont_text mft2;

//     vraminc_init();

//     multifont_init(&mf, MULTIFONT_TILES(&resource_palette_font_main), MULTIFONT_PAL(&resource_palette_font_main), BG_B, PAL0, &vraminc_get, 128);
//     // multifont_setPalette(&mf);

//     multifont_init_pal(&mft1, &mf);
//     multifont_init_pal(&mft2, &mf);

//     multifont_text_write(&mft1, "ABA C", 10, 10);
//     multifont_text_write(&mft2, "QERXX", 20, 11);
// }