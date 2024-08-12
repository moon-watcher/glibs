#pragma once

#include <genesis.h>

#define DISPLAY_PALETTES  4
#define DISPLAY_PALETTE_COLORS 16

#define DISPLAY_FADE_FUNCTION(FRAMES, COLORS)              \
    SYS_doVBlankProcess();                                 \
    SYS_disableInts();                                     \
                                                           \
    if (FRAMES)                                            \
        PAL_fadeTo(0, 63, (const u16 *)COLORS, FRAMES, 0); \
    else                                                   \
        PAL_setColors(0, (const u16 *)COLORS, 64, DMA);    \
                                                           \
    SYS_enableInts();
