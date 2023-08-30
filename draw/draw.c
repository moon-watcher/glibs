#include "include.h"
// #include "vram.h"

void drawText(const char *str, u16 x, u16 y)
{
    SYS_disableInts();
    VDP_drawText(str, x, y);
    SYS_enableInts();
}

void drawInt(u32 nb, u8 x, u8 y, u8 zeros)
{
    char str[zeros + 1];
    intToStr(nb, str, zeros);

    drawText(str, x, y);
}

void drawUInt(u32 nb, u8 x, u8 y, u8 zeros)
{
    char str[zeros + 1];
    uintToStr(nb, str, zeros);

    drawText(str, x, y);
}

void drawFix32(fix32 nb, u8 x, u8 y, u8 zeros)
{
    char str[16];

    fix32ToStr(nb, str, zeros);
    drawText(str, x, y);
}

void drawFix16(fix16 nb, u8 x, u8 y, u8 zeros)
{
    char str[16];

    fix16ToStr(nb, str, zeros);
    drawText(str, x, y);
}

void drawImage(int plan, const Image *const img, int pal)
{
    SYS_disableInts();
    VDP_drawImageEx(plan, img, TILE_ATTR_FULL(pal, 0, FALSE, FALSE, vraminc_get(img->tileset->numTile)), 0, 0, FALSE, TRUE);
    SYS_enableInts();
}

void drawImageA(const Image *img)
{
    drawImage(BG_A, img, PAL1);
}

void drawImageB(const Image *img)
{
    drawImage(BG_B, img, PAL0);
}

// void drawFadeInA ( const Image *img, u16 time ) {
//     drawFadeInA_XY ( img, time, 0, 0 );
// }

// void drawFadeInA_XY ( const Image *img, u16 time, int x, int y ) {
//     SYS_disableInts ( );
//     PAL_setPalette ( PAL1, palette_black, CPU );
//     VDP_drawImageEx ( BG_A, img, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, vrampos_get(img->tileset->numTile)), x, y, FALSE, TRUE );
//     SYS_enableInts ( );

//     if ( time ) {
//         PAL_fadeInPalette ( PAL1, img->palette->data, time, FALSE);
//     } else {
//         SYS_disableInts ( );
//         PAL_setPalette ( PAL1, img->palette->data, CPU );
//         SYS_enableInts ( );
//     }
// }