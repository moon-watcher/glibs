#include <genesis.h>

void SGDK_fade(unsigned short frames, unsigned short *colors)
{
    SYS_doVBlankProcess();
    SYS_disableInts();

    if (frames)
        PAL_fadeTo(0, 63, (const u16 *)colors, frames, 0);
    else
        PAL_setColors(0, (const u16 *)colors, 64, DMA);

    SYS_enableInts();
}